#include "section.hpp"
#include "symbol_table.hpp"
#include "misc.hpp"

s_section* new_section()
{
    return (s_section*)malloc(sizeof(s_section));
}

string get_section_symbol(s_section* section)
{
    if (section == 0)
        return "";

    s_symbol_table_entry* entry = get_symbol_entry_index(section->sym_table_index);
    return entry->name;
}

void write_bytes_to_section(s_section* section, vector<char> bytes)
{
    for (char byte : bytes)
    {
        section->bytes.push_back(byte);
    }
}

void skip_bytes_in_section(s_section* section, long num_bytes)
{
    for (long i = 0; i < num_bytes; i++)
    {
        section->bytes.push_back(0);
    }
}

void bind_rela_table_to_section(s_section* section, s_rela_table* rela_table)
{
    section->rela_table = rela_table;
    section->has_rela = true;
}

string section_to_string(s_section* section)
{
    string final_string = "#" + get_section_symbol(section) + "\n";

    int counter = 1;
    for (char c : section->bytes)
    {
        string hex = char_to_string_hex(c);
        final_string += hex + " ";

        if (counter % 8 == 0)
            final_string += "\n";

        counter++;
    }

    if (counter % 8 != 0)
        final_string += "\n";

    return final_string;
}

long get_section_offset(s_section* section)
{
    if (section == 0)
        return -1;

    return section->bytes.size();
}

s_section* import_section(int i_dont_know_yet)
{
    
}