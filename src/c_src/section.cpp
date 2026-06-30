#include "section.hpp"
#include "symbol_table.hpp"
#include "misc.hpp"
#include "asm.hpp"

s_section* new_section()
{
    s_section* section = new s_section();
    section->sym_table_index = -1;
    section->has_rela = false;
    section->rela_table = nullptr;
    return section;
}

string get_section_symbol(s_symbol_table* symbol_table, s_section* section)
{
    if (section == 0)
        return "";

    s_symbol_table_entry* entry = get_symbol_entry_index(symbol_table, section->sym_table_index);
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

string section_to_string(s_program* program, s_section* section)
{
    string final_string = "#" + get_section_symbol(get_symbol_table(program), section) + "\n";

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

s_section* find_section_by_name(vector<s_section>& section_list, s_symbol_table* symbol_table, string name)
{
    for (s_section& section : section_list)
    {
        if (get_section_symbol(symbol_table, &section) == name)
            return &section;
    }
    return nullptr;
}


s_section* import_section(vector<string> lines, s_symbol_table* symbol_table)
{
    if (lines.size() == 0)
        return nullptr;
    s_section* section = new_section();
    string section_name = lines[0].substr(1);

    s_symbol_table_entry* symbol = create_new_symbol_entry(symbol_table, section_name);

    symbol->binding = STB_LOCAL;
    symbol->offset_or_value = 0;
    symbol->section_symbol_table_index = get_symbol_entry_index_by_symbol(symbol_table, section_name);
    symbol->state = STS_COMPLETE;
    symbol->size = 0;
    symbol->type = STT_SECTION;

    section->sym_table_index = symbol->section_symbol_table_index;

    for (string line : lines)
    {
        vector<string> chars = split_string(line, ' ');

        for (string hex : chars)
        {
            char c = string_hex_to_char(hex);
            section->bytes.push_back(c);
        }
    }

    return section;
}