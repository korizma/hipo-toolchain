#include "section.hpp"
#include "symbol_table.hpp"
#include "misc.hpp"
#include "asm.hpp"
#include "linker.hpp"
#include "rela_table.hpp"

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

string section_to_string(s_symbol_table *symbol_table, s_section* section)
{
    string final_string = "#" + get_section_symbol(symbol_table, section) + "\n";

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

    s_symbol_table_entry* symbol = get_symbol_entry_symbol(symbol_table, section_name);

    symbol->binding = STB_LOCAL;
    symbol->offset_or_value = 0;
    symbol->section_symbol_table_index = get_symbol_entry_index_by_symbol(symbol_table, section_name);
    symbol->state = STS_COMPLETE;
    symbol->size = 0;
    symbol->type = STT_SECTION;

    section->sym_table_index = symbol->section_symbol_table_index;

    for (int i = 1; i < lines.size(); i++)
    {
        string line = lines[i];
        vector<string> chars = split_string(line, ' ');

        for (string hex : chars)
        {
            char c = string_hex_to_char(hex);
            section->bytes.push_back(c);
        }
    }

    return section;
}


void combine_all_sections(s_linker_state* linker_state)
{
    for (s_object_file& obj_file : linker_state->obj_files)
    {
        for (s_section& section : obj_file.sections)
        {
            combine_section_to_linked(linker_state, &section, &obj_file.symbol_table);
        }
    }
}

void combine_section_to_linked(s_linker_state* linker_state, s_section* s1, s_symbol_table* st1)
{
    s_section* main_section = get_section_by_name_from_linked_file(linker_state, get_section_symbol(st1, s1));

    if (main_section == nullptr)
    {
        // section with the same name doesnt exist
        // if it doesnt exist, we only add that section to the sections
        // update the section symbol table index
        // as well as all rela entries so that they have the good symbol table index
        add_section_to_linked_file(linker_state, s1, st1);
        return;
    }

    long increase = main_section->bytes.size();
    
    // we first update all symbols from this section, so they have increased value
    update_linked_symbol_table(linker_state, s1, st1, increase);

    // we update all rela entries for this section
    update_rela_table_linker(linker_state, s1, st1, increase);

    if (s1->has_rela)
    {
        if (!main_section->has_rela)
        {
            main_section->rela_table = new s_rela_table();
            main_section->has_rela = true;
        }
        for (s_rela_table_entry& rela_entry : s1->rela_table->entries)
        {
            main_section->rela_table->entries.push_back(rela_entry);
        }
    }

    // we write the bytes to the section
    write_bytes_to_section(main_section, s1->bytes);
}