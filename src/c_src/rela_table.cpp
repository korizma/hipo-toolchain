#include "rela_table.hpp"
#include "section.hpp"
#include <stdlib.h>
#include "symbol_table.hpp"
#include "misc.hpp"
#include "asm.hpp"

using namespace std;


s_rela_table* new_rela_table(s_section* section)
{
    if (section->has_rela)
    {
        return section->rela_table;
    }
    s_rela_table* rela = new s_rela_table();
    rela->section_symbol_table_index = section->sym_table_index;
    
    section->rela_table = rela;
    section->has_rela = true;
    return rela;
}

s_rela_table* get_rela_table(s_section* section)
{
    if (section->has_rela)
        return section->rela_table;
    else
        return nullptr;
}

s_rela_table_entry* create_new_rela_table_entry(s_section* section, long addend, long offset, char relocation_type, long symbol_index)
{
    s_rela_table_entry entry;
    entry.addend = addend;
    entry.offset_in_section = offset;
    entry.relocation_type = relocation_type;
    entry.symbol_symbol_table_index = symbol_index;
    
    s_rela_table* rela = get_rela_table(section);
    if (rela == nullptr)
        rela = new_rela_table(section);
    rela->entries.push_back(entry);

    return &rela->entries.back();
}

string rela_table_entry_to_string(s_rela_table_entry entry)
{
    string offset = long_to_string_hex(entry.offset_in_section);
    string type;
    if (entry.relocation_type == R_HIPO_12)
        type = "R_HIPO_12";
    else if (entry.relocation_type == R_HIPO_32)
        type = "R_HIPO_32";
    string symbol = to_string(entry.symbol_symbol_table_index) + " (" + get_symbol_entry_index(entry.symbol_symbol_table_index)->name + ")";
    string addend = to_string(entry.addend);

    return offset + "\t" + type + "\t" + symbol + "\t" + addend;
}

string rela_table_to_string(s_rela_table* rela_table)
{
    string final_string = "#.rela." + get_symbol_entry_index(rela_table->section_symbol_table_index)->name + "\n";

    final_string += "Offset\tType\tSymbol\tAddend\n";

    for (s_rela_table_entry entry : rela_table->entries)
    {
        final_string += rela_table_entry_to_string(entry) + "\n";
    }
    return final_string;
}

s_rela_table* import_rela_table(int i_dont_know_yet)
{
    return NULL;
}

void rela_table_symbol_execute_and_remove(s_symbol_table_entry* symbol)
{
    long symbol_index = get_symbol_entry_index_by_symbol(symbol->name);
    for (s_section& section : get_program()->section_list)
    {
        if (!section.has_rela)
            continue;

        s_rela_table* rela_table = section.rela_table;

        vector<int> to_remove;
        for (int i = 0; i < rela_table->entries.size(); i++)
        {
            s_rela_table_entry* entry = &rela_table->entries[i];

            if (entry->symbol_symbol_table_index == symbol_index)
            {
                to_remove.push_back(i);
                write_rela_table_entry(entry, &section);
            }
        }

        for (int i = to_remove.size()-1; i >= 0; i--)
        {
            rela_table->entries.erase(rela_table->entries.begin() + to_remove[i]);
        }
    }
}

void rela_table_symbol_update(s_symbol_table_entry* symbol)
{
    long symbol_index = get_symbol_entry_index_by_symbol(symbol->name);
    for (s_section section : get_program()->section_list)
    {
        if (!section.has_rela)
            continue;

        s_rela_table* rela_table = section.rela_table;

        for (int i = 0; i < rela_table->entries.size(); i++)
        {
            s_rela_table_entry* entry = &rela_table->entries[i];

            if (symbol->binding == STB_LOCAL && entry->symbol_symbol_table_index == symbol_index)
            {
                entry->addend = 0;
                entry->offset_in_section = symbol->offset_or_value;
                entry->symbol_symbol_table_index = symbol->section_symbol_table_index;
            }
        }
    }
}


void write_rela_table_entry(s_rela_table_entry* entry, s_section* section)
{
    s_symbol_table_entry* symbol = get_symbol_entry_index(entry->symbol_symbol_table_index);
    if (entry->relocation_type == R_HIPO_32)
    {
        vector<char> bytes = int_to_4_bytes(symbol->offset_or_value);
        section->bytes[entry->offset_in_section + 0] = bytes[0];
        section->bytes[entry->offset_in_section + 1] = bytes[1];
        section->bytes[entry->offset_in_section + 2] = bytes[2];
        section->bytes[entry->offset_in_section + 3] = bytes[3];
    }
    // i dont think im ever using the R_HIPO_12 relocation
}
