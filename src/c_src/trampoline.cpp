#include "trampoline.hpp"
#include "section.hpp"
#include "asm.hpp"
#include "error.hpp"
#include "misc.hpp"

//  adds a literal trampoline entry for the described section
void add_literal_trampoline_entry(s_section* section, long section_offset, long referent_offset, long literal)
{
    s_trampoline_entry entry;
    entry.literal = literal;
    entry.section_offset = section_offset;
    entry.referent_offset = referent_offset;
    entry.section = section;
    entry.type = TRMP_LIT;

    get_trampoline()->entries.push_back(entry);
}

// adds a symbol trampoline entry for the described section
void add_symbol_trampoline_entry(s_section* section, long section_offset, long referent_offset, long symbol_index)
{
    s_trampoline_entry entry;
    entry.section_offset = section_offset;
    entry.referent_offset = referent_offset;
    entry.section = section;
    entry.type = TRMP_SYM;
    entry.symbol_symbol_table_index = symbol_index;

    get_trampoline()->entries.push_back(entry);
}

s_error write_trampoline_entry(s_trampoline_entry* entry)
{
    if (entry->type == TRMP_LIT)
    {
        entry->trampoline_location = get_section_offset(entry->section);
        
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
    else if (entry->type = TRMP_SYM)
    {
        // for later
        // case if the symbol is EQU and has value less than 12bits, we can just put that as displacement
    }
}

vector<s_error> write_trampolines()
{
    s_trampoline* trampoline = get_trampoline();
    vector<s_error> errors;

    for (s_trampoline_entry entry : trampoline->entries)
    {
        s_error error = write_trampoline_entry(&entry);
        if (!error.no_error)
            errors.push_back(error);
    }

    return errors;
}