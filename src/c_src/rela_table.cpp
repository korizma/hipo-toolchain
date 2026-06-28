#include "rela_table.hpp"
#include "section.hpp"
#include <stdlib.h>
#include "symbol_table.hpp"

using namespace std;


s_rela_table* new_rela_table(s_section* section)
{
    if (section->has_rela)
    {
        return section->rela_table;
    }
    s_rela_table* rela = (s_rela_table*)malloc(sizeof(s_rela_table));
    rela->section_symbol_table_index = section->sym_table_index;
    
    section->rela_table = rela;
    section->has_rela = true;
    return rela;
}

s_rela_table* get_rela_table(s_section* section)
{
    return section->rela_table;
}

s_rela_table_entry* create_new_rela_table_entry(s_section* section, long addend, long offset, char relocation_type, long symbol_index)
{
    s_rela_table_entry entry;
    entry.addend = addend;
    entry.offset_in_section = offset;
    entry.relocation_type = relocation_type;
    entry.symbol_symbol_table_index = symbol_index;
    
    s_rela_table* rela = get_rela_table(section);
    rela->entries.push_back(entry);

    return &rela->entries.back();
}

string rela_table_to_string(s_rela_table* rela_table)
{
    return "";
}

s_rela_table* import_rela_table(int i_dont_know_yet)
{
    return NULL;
}
