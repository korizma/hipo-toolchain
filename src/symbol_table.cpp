#include "symbol_table.hpp"
#include "asm.hpp"
#include "error.hpp"
#include "expr.hpp"
#include "asm_instruction.hpp"
#include "section.hpp"
#include "misc.hpp"
#include "rela_table.hpp"
#include "linker.hpp"
#include "error.hpp"

s_symbol_table_entry* get_symbol_entry_index(s_symbol_table* table, int index)
{ 
    if (table->entries.size() <= index)
        return NULL;

    return &table->entries[index];
}

s_symbol_table_entry* get_symbol_entry_symbol(s_symbol_table* table, string symbol)
{
    for (int i = 0; i < table->entries.size(); i++)
    {
        if (table->entries[i].name == symbol)
            return &table->entries[i];
    }
    return NULL;
}

s_symbol_table_entry* create_new_symbol_entry(s_symbol_table* table, string symbol)
{
    for (s_symbol_table_entry& entry : table->entries)
    {
        if (entry.name == symbol)
            return NULL;
    }

    s_symbol_table_entry new_entry;
    new_entry.name = symbol;

    table->entries.push_back(new_entry);
    
    return &table->entries.back();
}

s_symbol_table_entry* get_and_create_new_symbol_entry(s_symbol_table* table, string symbol)
{
    s_symbol_table_entry* entry = create_new_symbol_entry(table, symbol);

    if (entry != NULL) return entry;

    for (int i = 0; i < table->entries.size(); i++)
    {
        if (table->entries[i].name == symbol)
            return &table->entries[i];
    }

    return NULL;
}

long get_symbol_entry_index_by_symbol(s_symbol_table* table, string symbol)
{    
    for (int i = 0; i < table->entries.size(); i++)
    {
        s_symbol_table_entry entry = table->entries[i];
        if (entry.name == symbol)
            return i;
    }
    return -1;
}

string symbol_table_entry_to_string(s_symbol_table_entry entry, long index)
{
    string num = to_string(index) + ":";
    string value = to_string(entry.offset_or_value);
    string size = to_string(entry.size);
    string type;
    if (entry.type == STT_NOTYPE)
        type = "NOTYP";
    else if (entry.type == STT_SECTION)
        type = "SCTN";
    string bind;
    if (entry.binding == STB_GLOBAL)
        bind = "GLOB";
    else if (entry.binding == STB_LOCAL)
        bind = "LOC";
    string ndx = to_string(entry.section_symbol_table_index);
    if (entry.section_symbol_table_index == -1)
        ndx = "UND";
    string name = entry.name;

    return num + "\t" + value + "\t" + size + "\t" + type + "\t" + bind + "\t" + ndx + "\t" + name;
}

string symbol_table_to_string(s_symbol_table* table)
{
    string name = "#.symtab";
    string header = "Num\tValue\tSize\tType\tBind\tNdx\tName";

    string final_string = name + "\n" + header + "\n";

    for (int i = 0; i < table->entries.size(); i++)
    {
        s_symbol_table_entry entry = table->entries[i];
        final_string += symbol_table_entry_to_string(entry, i) + "\n";
    }

    return final_string;
}

vector<s_error> _find_undefined_symbols_symbol_table(s_symbol_table* table)
{
    vector<s_error> errs;
    for (s_symbol_table_entry& symbol : table->entries)
    {
        if (symbol.state != STS_COMPLETE && symbol.state != STS_EQU)
        {
            errs.push_back(new_error(ERR_UNDEFINED_SYM, symbol.name));
        }
    }
    return errs;
}

void _update_rela_table_for_symbols_symbol_table(s_program* program, s_symbol_table* table)
{
    for (s_symbol_table_entry& symbol : table->entries)
    {
        rela_table_symbol_update(get_symbol_table(program), program->section_list, &symbol);
    }
}

vector<s_error> _finalize_equ_symbols_symbol_table(s_program* program, s_symbol_table* table)
{
    vector<s_error> errs;
    for (s_symbol_table_entry& symbol : table->entries)
    {
        if (symbol.state == STS_EQU)
        {
            equ_set_symbol_indexes(program, symbol.expression);

            if (symbol.expression->undefined_symbol_exists)
            {
                errs.push_back(new_error(ERR_UNDEFINED_SYM, symbol.expression->symbol[0]));
                continue;
            }
            simplify_expression(program, symbol.expression);
            
            if (symbol.expression->undefined_symbol_exists)
            {
                errs.push_back(new_error(ERR_UNDEFINED_SYM, symbol.expression->symbol[0]));
                continue;
            }

            if (symbol.expression->symbol.size() == 0)
            {
                // symbol is just literal
                symbol.offset_or_value = symbol.expression->integer_value;
            }
            else if (symbol.expression->symbol.size() == 1)
            {
                s_symbol_table_entry* ref = get_symbol_entry_index(table, symbol.expression->symbol_index[0]);
                long coeff = symbol.expression->coeff[0];

                symbol.section_symbol_table_index = ref->section_symbol_table_index;

                symbol.offset_or_value = ref->offset_or_value * coeff + symbol.expression->integer_value;
            }
            else 
            {
                errs.push_back(new_error(ERR_EQU_NOT_CALCULATABLE, symbol.name));
                continue;
            }
        }
    }
    return errs;
}

void _rela_equ_updating(s_program* program, s_symbol_table* table)
{
    for (s_symbol_table_entry& symbol : table->entries)
    {
        if (symbol.state == STS_EQU)
        {
            // if its only a literal, then find all rela and do them
            if (symbol.expression->symbol.size() == 0)
            {
                rela_table_symbol_execute_and_remove(get_symbol_table(program), program->section_list, &symbol);
            }
            // if its not, then you need to edit the rela entries if the visibility is not global
            else if (symbol.binding == STB_LOCAL)
            {
                rela_table_symbol_update(get_symbol_table(program), program->section_list, &symbol);
            }
        }
    }
}

vector<s_error> _check_equ_mem_reg_sym_addressing(s_symbol_table* table)
{
    vector<s_error> errs;
    for (s_symbol_table_entry& symbol : table->entries)
    {
        if (symbol.state == STS_EQU)
        {
            for (s_mem_reg_sym rs : symbol.mem_reg_sym_list)
            {
                if (symbol.expression->symbol.size() == 0)
                {
                    if (long_fits_in_12bits(symbol.offset_or_value))
                        write_to_lower_12b(rs.section->bytes, rs.offset, symbol.offset_or_value);
                    else
                        errs.push_back(new_error(ERR_SYMBOL_TOO_LARGE_LS, symbol.name));
                }
                else
                {
                    errs.push_back(new_error(ERR_SYMBOL_TOO_LARGE_LS, asm_instruction_to_string(rs.instruction)));
                }
            }
        }
        else if (symbol.mem_reg_sym_list.size() != 0)
        {
            for (s_mem_reg_sym rs : symbol.mem_reg_sym_list)
            {
                errs.push_back(new_error(ERR_SYMBOL_TOO_LARGE_LS, asm_instruction_to_string(rs.instruction)));
            }
        }
    }
    return errs;
}

void _update_section_sizes(s_program* program)
{
    for (s_section& section : program->section_list)
    {
        s_symbol_table_entry* entry = get_symbol_entry_index(get_symbol_table(program), section.sym_table_index);
        if (entry != NULL)
            entry->size = section.bytes.size();
    }
}

vector<s_error> finalize_symbol_table(s_program* program)
{
    s_symbol_table* table = get_symbol_table(program);
    vector<s_error> errs = _find_undefined_symbols_symbol_table(table);
    if (errs.size() != 0)
        return errs;
    
    errs = _finalize_equ_symbols_symbol_table(program, table);
    if (errs.size() != 0)
        return errs;


    errs = _check_equ_mem_reg_sym_addressing(table);
    if (errs.size() != 0)
            return errs;

    _rela_equ_updating(program, table);
    _update_rela_table_for_symbols_symbol_table(program, table);

    _update_section_sizes(program);

    return {};
}

void add_mem_reg_sym_to_symbol(s_symbol_table_entry* symbol, s_section* section, long offset, s_asm_instruction* instruction)
{
    s_mem_reg_sym new_mem_reg_sym;
    new_mem_reg_sym.section = section;
    new_mem_reg_sym.offset = offset;
    new_mem_reg_sym.instruction = instruction;
    symbol->mem_reg_sym_list.push_back(new_mem_reg_sym);
}

//  0     1    2       3     4    5    6
// "Num\tValue\tSize\tType\tBind\tNdx\tName";
s_symbol_table_entry import_symbol_table_entry(string line)
{
    vector<string> parts = split_string(line, '\t');

    s_symbol_table_entry entry;
    if (parts.size() != 7)
        return entry;

    if (parts[3] == "SCTN")
        entry.type = STT_SECTION;
    if (parts[3] == "NOTYP")
        entry.type = STT_NOTYPE;

    entry.offset_or_value = stol(parts[1], nullptr, 10);
    if (parts[5] == "UND")
        entry.section_symbol_table_index = -1;
    else
        entry.section_symbol_table_index = stoi(parts[5], nullptr, 10);
    entry.size = stol(parts[2], nullptr, 10);
    entry.name = parts[6];

    if (parts[4] == "LOC")
        entry.binding = STB_LOCAL;
    if (parts[4] == "GLOB")
        entry.binding = STB_GLOBAL;

    return entry;
}

s_symbol_table* import_symbol_table(vector<string> lines)
{
    string header = lines[0];
    if (lines.size() < 3 || header != "#.symtab" || lines[1] != "Num\tValue\tSize\tType\tBind\tNdx\tName")
        return NULL;
    
    s_symbol_table* sym_table = new s_symbol_table();

    for (int i = 2; i < lines.size(); i++)
    {
        s_symbol_table_entry entry = import_symbol_table_entry(lines[i]);
        sym_table->entries.push_back(entry);
    }
    return sym_table;
}

// returns conflicts
vector<s_error> _add_symbols_to_sym_table(s_symbol_table* main_table, s_symbol_table* side_table)
{
    vector<s_error> conflicts;
    // all cases
    // for normal symbols, if they have a conflict error
    // for extern symbols, they add them self if they dont exist, but dont change anything if it already exists
    for (s_symbol_table_entry& entry : side_table->entries)
    {
        s_symbol_table_entry* existing = get_symbol_entry_symbol(main_table, entry.name);
        if (entry.type == STT_SECTION)
        {
            if (existing == nullptr)
            {
                s_symbol_table_entry* new_entry = create_new_symbol_entry(main_table, entry.name);
                new_entry->binding = entry.binding;
                new_entry->offset_or_value = entry.offset_or_value;
                new_entry->section_symbol_table_index = get_symbol_entry_index_by_symbol(main_table, side_table->entries[entry.section_symbol_table_index].name);
                new_entry->size = entry.size;
                new_entry->type = entry.type;
            }
            else
            {
                existing->binding = entry.binding;
                existing->offset_or_value = entry.offset_or_value;
                existing->section_symbol_table_index = get_symbol_entry_index_by_symbol(main_table, side_table->entries[entry.section_symbol_table_index].name);
                existing->size = entry.size;
                existing->type = entry.type;
            }
        }
        else if (entry.binding == STB_GLOBAL && entry.section_symbol_table_index == -1 && entry.offset_or_value == 0)
        {
            // this symbol is extern
            if (existing == nullptr)
            {
                s_symbol_table_entry* new_entry = create_new_symbol_entry(main_table, entry.name);
                new_entry->binding = entry.binding;
                new_entry->offset_or_value = entry.offset_or_value;
                new_entry->section_symbol_table_index = entry.section_symbol_table_index;
                new_entry->size = entry.size;
                new_entry->type = entry.type;
            }
            else
            {
                // if the binding is local, then its an error
                if (existing->binding == STB_LOCAL)
                {
                    conflicts.push_back(new_error(ERR_EXTERN_LOCAL_CONFLICT, entry.name));
                }
            }
        }
        else
        {
            if (existing != nullptr)
            {
                // if its not an extern symbol, error
                if (!(existing->binding == STB_GLOBAL && existing->section_symbol_table_index == -1 && existing->offset_or_value == 0))
                {
                    // not extern symbol
                    conflicts.push_back(new_error(ERR_SYMBOL_CONFLICT, entry.name));
                }
                existing->binding = entry.binding;
                existing->offset_or_value = entry.offset_or_value;
                if (entry.section_symbol_table_index == -1)
                    existing->section_symbol_table_index = -1;
                else
                    existing->section_symbol_table_index = get_symbol_entry_index_by_symbol(main_table, side_table->entries[entry.section_symbol_table_index].name);
                existing->size = entry.size;
                existing->type = entry.type;
            }
            else
            {
                s_symbol_table_entry* new_entry = create_new_symbol_entry(main_table, entry.name);
                new_entry->binding = entry.binding;
                new_entry->offset_or_value = entry.offset_or_value;
                if (entry.section_symbol_table_index == -1)
                    new_entry->section_symbol_table_index = -1;
                else
                    new_entry->section_symbol_table_index = get_symbol_entry_index_by_symbol(main_table, side_table->entries[entry.section_symbol_table_index].name);
                new_entry->size = entry.size;
                new_entry->type = entry.type;
            }
        }
    }

    return conflicts;
}

// combines all symbol tables into a global symbol table, returns conflicts of symbols
vector<s_error> combine_all_symbol_tables_rel(s_linker_state* linker_state)
{
    vector<s_error> errs;
    for (s_object_file& obj_file : linker_state->obj_files)
    {
        vector<s_error> new_errs = _add_symbols_to_sym_table(&linker_state->linked_file.symbol_table, &obj_file.symbol_table);

        errs.insert(errs.end(), new_errs.begin(), new_errs.end());
    }
    return errs;
}

// increases the offset for each symbol from the old symbol table by increase
void update_linked_symbol_table(s_linker_state* linker_state, s_section* section, s_symbol_table* old_symbol_table, long increase)
{
    for (s_symbol_table_entry& entry : old_symbol_table->entries)
    {
        // skip all non section symbols
        if (entry.section_symbol_table_index != section->sym_table_index)
            continue;

        s_symbol_table_entry* new_entry = get_symbol_entry_symbol(get_symbol_table_linker(linker_state), entry.name);

        new_entry->offset_or_value += increase;
    }
}

vector<s_error> find_unresolved_symbols(s_symbol_table* symbol_table)
{
    vector<s_error> unresolved;

    for (s_symbol_table_entry& entry : symbol_table->entries)
    {
        if (entry.type == STT_NOTYPE && entry.binding == STB_GLOBAL && entry.offset_or_value == 0 && entry.section_symbol_table_index == -1)
            unresolved.push_back(new_error(ERR_UNDEFINED_SYM, entry.name));
    }

    return unresolved;
}


void resolve_all_symbols(s_linker_state* linker_state)
{
    s_symbol_table* symbol_table = get_symbol_table_linker(linker_state);
    for (s_symbol_table_entry& entry : symbol_table->entries)
    {
        if (entry.type == STT_SECTION || entry.section_symbol_table_index == -1)
            continue;

        s_symbol_table_entry* section = get_symbol_entry_index(symbol_table, entry.section_symbol_table_index);

        entry.offset_or_value += section->offset_or_value;
    }
}
