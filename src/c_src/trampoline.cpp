#include "trampoline.hpp"
#include "section.hpp"
#include "asm.hpp"
#include "error.hpp"
#include "misc.hpp"
#include "symbol_table.hpp"
#include "rela_table.hpp"

//  adds a literal trampoline entry for the described section
void add_literal_trampoline_entry(s_program* program, s_section* section, long section_offset, long referent_offset, long literal)
{
    s_trampoline_entry entry;
    entry.literal = literal;
    entry.section_offset = section_offset;
    entry.referent_offset = referent_offset;
    entry.section = section;
    entry.type = TRMP_LIT;

    get_trampoline(program)->entries.push_back(entry);
}

// adds a symbol trampoline entry for the described section
void add_symbol_trampoline_entry(s_program* program, s_section* section, long section_offset, long referent_offset, long symbol_index)
{
    s_trampoline_entry entry;
    entry.section_offset = section_offset;
    entry.referent_offset = referent_offset;
    entry.section = section;
    entry.type = TRMP_SYM;
    entry.symbol_symbol_table_index = symbol_index;
    entry.trampoline_location = 0;

    get_trampoline(program)->entries.push_back(entry);
}

s_error write_trampoline_entry(s_program* program, s_trampoline_entry* entry)
{
    if (entry->type == TRMP_LIT)
    {
        long trampoline_location = get_trampoline_location_if_exists_literal(program, entry->literal);

        if (trampoline_location == -1)
            entry->trampoline_location = get_section_offset(entry->section);
        else
            entry->trampoline_location = trampoline_location;
        
        vector<char> bytes = int_to_4_bytes(entry->literal);
        write_bytes_to_section(entry->section, bytes);

        int displacement = entry->trampoline_location - entry->referent_offset;

        if (!long_fits_in_12bits(displacement))
        {
            return new_error(ERR_TRAMP_OFFSET_12b);
        }

        write_to_lower_12b(entry->section->bytes, entry->section_offset, displacement);

        return new_no_error();
    }
    else if (entry->type == TRMP_SYM)
    {
        // for later
        // case if the symbol is EQU and has value less than 12bits, we can just put that as displacement

        s_symbol_table_entry* symbol = get_symbol_entry_index(get_symbol_table(program), entry->symbol_symbol_table_index);

        
            long trampoline_location = get_trampoline_location_if_exists_symbol(program, entry->symbol_symbol_table_index);

            if (trampoline_location != -1)
            {
                entry->trampoline_location = trampoline_location;
            }
            else
            {
                entry->trampoline_location = get_section_offset(entry->section);
                skip_bytes_in_section(entry->section, 4);
                if (symbol->binding == STB_GLOBAL)
                {
                    create_new_rela_table_entry(entry->section, 0, entry->trampoline_location, R_HIPO_32, entry->symbol_symbol_table_index);
                }
                else if (symbol->binding == STB_LOCAL)
                {
                    create_new_rela_table_entry(
                        entry->section,
                        symbol->offset_or_value,
                        entry->trampoline_location,
                        R_HIPO_32,
                        symbol->section_symbol_table_index
                    );    
                }
            }


            int displacement = entry->trampoline_location - entry->referent_offset;

            if (!long_fits_in_12bits(displacement))
            {
                return new_error(ERR_TRAMP_OFFSET_12b);
            }

            write_to_lower_12b(entry->section->bytes, entry->section_offset, displacement);

            return new_no_error();
        

        return new_no_error();
    }

    return new_no_error();
}

vector<s_error> write_trampolines(s_program* program)
{
    s_trampoline* trampoline = get_trampoline(program);
    vector<s_error> errors;

    for (s_trampoline_entry& entry : trampoline->entries)
    {
        s_error error = write_trampoline_entry(program, &entry);
        if (!error.no_error)
            errors.push_back(error);
    }

    return errors;
}


long get_trampoline_location_if_exists_literal(s_program* program, long literal)
{
    for (s_trampoline_entry entry : get_trampoline(program)->entries)
    {
        if (entry.literal == literal && entry.trampoline_location != 0)
            return entry.trampoline_location;
    }
    return -1;
}

long get_trampoline_location_if_exists_symbol(s_program* program, long symbol_index)
{
    for (s_trampoline_entry entry : get_trampoline(program)->entries)
    {
        if (entry.symbol_symbol_table_index == symbol_index && entry.trampoline_location != 0)
            return entry.trampoline_location;
    }
    return -1;
}
