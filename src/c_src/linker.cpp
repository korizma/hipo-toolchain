#include "linker.hpp"
#include "object_file.hpp"
#include "symbol_table.hpp"
#include "rela_table.hpp"
#include "section.hpp"
#include <iostream>
#include <fstream>

using namespace std;

bool linker_execute(char type, vector<string> obj_filenames, string output_filename, pair<string, long> section_place_locations)
{
    s_linker_state* linker_state = new_linker(type, obj_filenames, output_filename, section_place_locations);

    bool success = false;
    if (type == LINKER_HEX)
    {
        success = export_linked_file_hex(linker_state);
    }
    else if (type == LINKER_REL)
    {
        success = export_linked_file_rel(linker_state);
    }

    if (!success)
        cout << "Linking failed." << endl;

    return success;
}

s_linker_state* new_linker(char type, vector<string> obj_filenames, string output_filename, pair<string, long> section_place_locations)
{
    s_linker_state* linker_state = new s_linker_state();
    linker_state->linker_type = type;
    linker_state->output_filename = output_filename;
    linker_state->section_place_locations = section_place_locations;

    for (const auto& filename : obj_filenames)
    {
        s_object_file obj_file = load_object_file(filename);
        linker_state->obj_files.push_back(obj_file);
    }

    return linker_state;
}

bool export_linked_file_rel(s_linker_state* linker_state)
{
    vector<string> symbol_conflicts = combine_all_symbol_tables_rel(linker_state);

    if (symbol_conflicts.size() != 0)
    {
        cout << "Symbol Conclicts: " << endl;
        for (string symbol : symbol_conflicts)
            cout << symbol << endl;
        
        return false;
    }

    combine_all_sections(linker_state);

    string output = output_linked_to_string_rel(linker_state);

    ofstream file(linker_state->output_filename);
    if (!file.is_open())
    {
        cout << "Failed to open output file: " << linker_state->output_filename << endl;
        return false;
    }

    file << output;
    file.close();

    return true;
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


bool export_linked_file_hex(s_linker_state* linker_state)
{

}