#include "symbol_table.hpp"
#include "asm.hpp"
#include "error.hpp"
#include "expr.hpp"
#include "asm_instruction.hpp"
#include "section.hpp"
#include "misc.hpp"
#include "rela_table.hpp"

s_symbol_table_entry* get_symbol_entry_index(int index)
{
    s_symbol_table* table = get_symbol_table(); 
    if (table->entries.size() <= index)
        return NULL;

    return &table->entries[index];
}

s_symbol_table_entry* get_symbol_entry_symbol(string symbol)
{
    s_symbol_table* table = get_symbol_table(); 

    for (int i = 0; i < table->entries.size(); i++)
    {
        if (table->entries[i].name == symbol)
            return &table->entries[i];
    }
    return NULL;
}

s_symbol_table_entry* create_new_symbol_entry(string symbol)
{
    s_symbol_table* table = get_symbol_table(); 

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

s_symbol_table_entry* get_and_create_new_symbol_entry(string symbol)
{
    s_symbol_table_entry* entry = create_new_symbol_entry(symbol);

    if (entry != NULL) return entry;
    
    s_symbol_table* table = get_symbol_table(); 

    for (int i = 0; i < table->entries.size(); i++)
    {
        if (table->entries[i].name == symbol)
            return &table->entries[i];
    }

    return NULL;
}

long get_symbol_entry_index_by_symbol(string symbol)
{    
    s_symbol_table* table = get_symbol_table(); 

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

string symbol_table_to_string()
{
    string name = "#.symtab";
    string header = "Num\tValue\tSize\tType\tBind\tNdx\tName";

    string final_string = name + "\n" + header + "\n";

    for (int i = 0; i < get_symbol_table()->entries.size(); i++)
    {
        s_symbol_table_entry entry = get_symbol_table()->entries[i];
        final_string += symbol_table_entry_to_string(entry, i) + "\n";
    }

    return final_string;
}

vector<s_error> _find_undefined_symbols_symbol_table()
{
    vector<s_error> errs;
    for (s_symbol_table_entry& symbol : get_symbol_table()->entries)
    {
        if (symbol.state != STS_COMPLETE && symbol.state != STS_EQU)
        {
            errs.push_back(new_error(ERR_UNDEFINED_SYM, symbol.name));
        }
    }
    return errs;
}

void _update_rela_table_for_symbols_symbol_table()
{
    for (s_symbol_table_entry& symbol : get_symbol_table()->entries)
    {
        rela_table_symbol_update(&symbol);
    }
}

vector<s_error> _finalize_equ_symbols_symbol_table()
{
    vector<s_error> errs;
    for (s_symbol_table_entry& symbol : get_symbol_table()->entries)
    {
        if (symbol.state == STS_EQU)
        {
            equ_set_symbol_indexes(symbol.expression);
            simplify_expression(symbol.expression);

            

            if (symbol.expression->symbol.size() == 0)
            {
                // symbol is just literal
                symbol.offset_or_value = symbol.expression->integer_value;
            }
            else if (symbol.expression->symbol.size() == 1)
            {
                s_symbol_table_entry* ref = get_symbol_entry_index(symbol.expression->symbol_index[0]);
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

vector<s_error> _rela_equ_updating()
{
    vector<s_error> errs;
    for (s_symbol_table_entry& symbol : get_symbol_table()->entries)
    {
        if (symbol.state == STS_EQU)
        {
            // if its only a literal, then find all rela and do them
            if (symbol.expression->symbol.size() == 0)
            {
                rela_table_symbol_execute_and_remove(&symbol);
            }
            // if its not, then you need to edit the rela entries if the visibility is not global
            else if (symbol.binding == STB_LOCAL)
            {
                rela_table_symbol_update(&symbol);
            }
        }
    }
    return errs;
}

vector<s_error> _check_equ_mem_reg_sym_addressing()
{
    vector<s_error> errs;
    for (s_symbol_table_entry& symbol : get_symbol_table()->entries)
    {
        if (symbol.state == STS_EQU)
        {
            for (s_mem_reg_sym rs : symbol.mem_reg_sym_list)
            {
                if (symbol.expression->symbol.size() == 0)
                {
                    write_to_lower_12b(rs.section->bytes, rs.offset, symbol.offset_or_value);
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

vector<s_error> finalize_symbol_table()
{
    vector<s_error> undefined_symbol_errs = _find_undefined_symbols_symbol_table();
    vector<s_error> equ_symbol_errs = _finalize_equ_symbols_symbol_table();
    vector<s_error> mem_reg_sym_errs = _check_equ_mem_reg_sym_addressing();

    undefined_symbol_errs.insert(undefined_symbol_errs.end(), equ_symbol_errs.begin(), equ_symbol_errs.end());
    undefined_symbol_errs.insert(undefined_symbol_errs.end(), mem_reg_sym_errs.begin(), mem_reg_sym_errs.end());

    return undefined_symbol_errs;
}

void add_mem_reg_sym_to_symbol(s_symbol_table_entry* symbol, s_section* section, long offset, s_asm_instruction* instruction)
{
    s_mem_reg_sym new_mem_reg_sym;
    new_mem_reg_sym.section = section;
    new_mem_reg_sym.offset = offset;
    new_mem_reg_sym.instruction = instruction;
    symbol->mem_reg_sym_list.push_back(new_mem_reg_sym);
}
