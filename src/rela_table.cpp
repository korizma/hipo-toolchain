#include "rela_table.hpp"
#include "section.hpp"
#include <stdlib.h>
#include "symbol_table.hpp"
#include "misc.hpp"
#include "asm.hpp"
#include "linker.hpp"

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
    string symbol = to_string(entry.symbol_symbol_table_index);
    string addend = to_string(entry.addend);

    return offset + "\t" + type + "\t" + symbol + "\t" + addend;
}

string rela_table_to_string(s_symbol_table* symbol_table, s_rela_table* rela_table)
{
    string final_string = "#.rela." + get_symbol_entry_index(symbol_table, rela_table->section_symbol_table_index)->name + "\n";

    final_string += "Offset\tType\tSymbol\tAddend\n";

    for (s_rela_table_entry entry : rela_table->entries)
    {
        final_string += rela_table_entry_to_string(entry) + "\n";
    }
    return final_string;
}


void rela_table_symbol_execute_and_remove(s_symbol_table* symbol_table, vector<s_section>& section_list, s_symbol_table_entry* symbol)
{
    long symbol_index = get_symbol_entry_index_by_symbol(symbol_table, symbol->name);
    for (s_section& section : section_list)
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
                write_rela_table_entry(symbol_table, entry, &section);
            }
        }

        for (int i = to_remove.size()-1; i >= 0; i--)
        {
            rela_table->entries.erase(rela_table->entries.begin() + to_remove[i]);
        }
    }
}

void rela_table_symbol_update(s_symbol_table* symbol_table, vector<s_section>& section_list, s_symbol_table_entry* symbol)
{
    long symbol_index = get_symbol_entry_index_by_symbol(symbol_table, symbol->name);
    for (s_section section : section_list)
    {
        if (!section.has_rela)
            continue;

        s_rela_table* rela_table = section.rela_table;

        for (int i = 0; i < rela_table->entries.size(); i++)
        {
            s_rela_table_entry* entry = &rela_table->entries[i];

            if (symbol->binding == STB_LOCAL && entry->symbol_symbol_table_index == symbol_index)
            {
                entry->addend = symbol->offset_or_value;
                entry->symbol_symbol_table_index = symbol->section_symbol_table_index;
            }
        }
    }
}


void write_rela_table_entry(s_symbol_table* symbol_table, s_rela_table_entry* entry, s_section* section)
{
    int offset = 0;
    if (entry->symbol_symbol_table_index != -1)
        offset = get_symbol_entry_index(symbol_table, entry->symbol_symbol_table_index)->offset_or_value;

    if (entry->relocation_type == R_HIPO_32)
    {
        vector<char> bytes = int_to_4_bytes(offset + entry->addend);
        section->bytes[entry->offset_in_section + 0] = bytes[0];
        section->bytes[entry->offset_in_section + 1] = bytes[1];
        section->bytes[entry->offset_in_section + 2] = bytes[2];
        section->bytes[entry->offset_in_section + 3] = bytes[3];
    }
    // i dont think im ever using the R_HIPO_12 relocation
}

s_rela_table_entry import_rela_table_entry(string text)
{
    vector<string> parts = split_string(text, '\t');

    s_rela_table_entry entry;
    if (parts.size() != 4)
        return entry;

    entry.offset_in_section = stol(parts[0], nullptr, 16);
    entry.relocation_type = R_HIPO_32;
    entry.addend = stol(parts[3], nullptr, 10);
    entry.symbol_symbol_table_index = stoi(parts[2], nullptr, 10);

    return entry;
}

s_rela_table* import_rela_table(vector<string> lines, s_symbol_table* symbol_table, vector<s_section>& section_list)
{
    string header = lines[0];
    if (lines.size() < 3 || header.substr(0, 7) != "#.rela." || lines[1] != "Offset\tType\tSymbol\tAddend")
        return NULL;
    string section_name = header.substr(7);

    s_rela_table* rela_table = new s_rela_table();
    rela_table->section_symbol_table_index = get_symbol_entry_index_by_symbol(symbol_table, section_name);
    s_section* section = find_section_by_name(section_list, symbol_table, section_name);
    section->has_rela = true;
    section->rela_table = rela_table;
    
    for (int i = 2; i < lines.size(); i++)
    {
        s_rela_table_entry entry = import_rela_table_entry(lines[i]);
        rela_table->entries.push_back(entry);
    }
    return rela_table;
}


void update_rela_table_linker(s_linker_state* linker_state, s_section* s1, s_symbol_table* st1, long increase)
{
    if (!s1->has_rela)
        return;

    for (s_rela_table_entry& entry : s1->rela_table->entries)
    {
        entry.offset_in_section += increase;
        if (entry.symbol_symbol_table_index != -1)
            entry.symbol_symbol_table_index = get_symbol_entry_index_by_symbol(get_symbol_table_linker(linker_state), get_symbol_entry_index(st1, entry.symbol_symbol_table_index)->name);
    }

    s1->rela_table->section_symbol_table_index = s1->sym_table_index;
}


void write_all_relocations(s_linker_state* linker_state)
{
    for (s_section& section : linker_state->linked_file.sections)
    {
        if (!section.has_rela)
            continue;

        for (s_rela_table_entry& entry : section.rela_table->entries)
        {
            write_rela_table_entry(get_symbol_table_linker(linker_state), &entry, &section);
        }
    }
}
