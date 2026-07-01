#include "linker.hpp"
#include "object_file.hpp"
#include "symbol_table.hpp"
#include "rela_table.hpp"
#include "section.hpp"
#include "error.hpp"
#include <iostream>
#include <fstream>

using namespace std;

bool linker_execute(char type, vector<string> obj_filenames, string output_filename, vector<pair<string, long>> section_place_locations)
{
    s_linker_state* linker_state = new_linker(type, obj_filenames, output_filename, section_place_locations);

    if (linker_state->failed_load)
    {
        cout << "Loading object files failed." << endl;
        return false;
    }

    bool success = false;
    if (type == LINKER_HEX)
    {
        s_error error = export_linked_file_hex(linker_state);
        if (!error.no_error)
        {
            cout << "Linking failed: " << error_to_string(error) << endl;
        }
        success = error.no_error;
    }
    else if (type == LINKER_REL)
    {
        s_error error = export_linked_file_rel(linker_state);
        if (!error.no_error)
        {
            cout << "Linking failed: " << error_to_string(error) << endl;
        }
        success = error.no_error;
    }

    return success;
}

s_linker_state* new_linker(char type, vector<string> obj_filenames, string output_filename, vector<pair<string, long>> section_place_locations)
{
    s_linker_state* linker_state = new s_linker_state();
    linker_state->linker_type = type;
    linker_state->output_filename = output_filename;
    linker_state->section_place_locations = section_place_locations;
    linker_state->failed_load = false;

    for (const auto& filename : obj_filenames)
    {
        s_object_file* obj_file = load_object_file(linker_state, filename);
        if (obj_file)
            linker_state->obj_files.push_back(*obj_file);
        else
            linker_state->failed_load = true;
    }

    return linker_state;
}

s_error export_linked_file_rel(s_linker_state* linker_state)
{
    vector<s_error> symbol_conflicts = combine_all_symbol_tables_rel(linker_state);

    if (symbol_conflicts.size() != 0)
    {
        cout << "Symbol Conclicts: " << endl;
        for (s_error symbol : symbol_conflicts)
            cout << error_to_string(symbol) << endl;
        
        return new_error(ERR_LINK_FAIL_SYM_CONFLICT);
    }

    combine_all_sections(linker_state);

    string output = output_linked_to_string_rel(linker_state);

    ofstream file(linker_state->output_filename);
    if (!file.is_open())
    {
        cout << "Failed to open output file: " << linker_state->output_filename << endl;
        return new_error(ERR_OUTPUT_FILE_OPEN_FAIL);
    }

    file << output;
    file.close();

    return new_no_error();
}

vector<s_object_file> get_object_files(s_linker_state* linker_state)
{
    return linker_state->obj_files;
}

s_symbol_table* get_symbol_table_linker(s_linker_state* linker_state)
{
    return &linker_state->linked_file.symbol_table;
}

string output_linked_to_string_hex(s_linker_state* linker_state)
{
    return "";
}

string output_linked_to_string_rel(s_linker_state* linker_state)
{
    string symbol_table = symbol_table_to_string(&linker_state->linked_file.symbol_table);

    string sections = "";

    for (s_section& section : linker_state->linked_file.sections)
    {
        sections += section_to_string(&linker_state->linked_file.symbol_table, &section);
        if (section.has_rela)
            sections += rela_table_to_string(&linker_state->linked_file.symbol_table, section.rela_table);
    }

    return symbol_table + sections;
}


s_error export_linked_file_hex(s_linker_state* linker_state)
{
    // first merge all symbol tables
    // combine sections
    // then place sections, first place using the place commands, then by defined order
    // resolve symbol values
    // do relocations
    // export
}


s_section* get_section_by_name_from_linked_file(s_linker_state* linker_state, string name)
{
    for (int i = 0; i < linker_state->linked_file.sections.size(); i++)
    {
        string section_name = get_section_symbol(get_symbol_table_linker(linker_state), &linker_state->linked_file.sections[i]);
        if (section_name == name)
            return &linker_state->linked_file.sections[i];
    }
    return nullptr;
}

void add_section_to_linked_file(s_linker_state* linker_state, s_section* section, s_symbol_table* old_st)
{
    section->sym_table_index = get_symbol_entry_index_by_symbol(get_symbol_table_linker(linker_state), get_section_symbol(old_st, section));
    linker_state->linked_file.sections.push_back(*section);

    update_rela_table_linker(linker_state, section, old_st, 0);
}