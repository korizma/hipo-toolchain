#pragma once

#include <vector>
#include <string>
#include <utility>

#include "object_file.hpp"

using namespace std;

#define LINKER_HEX 1
#define LINKER_REL 2

typedef struct s_linker_state
{
    vector<s_object_file> obj_files;

    s_object_file linked_file;

    char linker_type;

    string output_filename;

    vector<pair<string, long>> section_place_locations;

    bool failed_load;

} s_linker_state;

// executes the linker, and links all files, outputs to the output file
bool linker_execute(char type, vector<string> obj_filenames, string output_filename, vector<pair<string, long>> section_place_locations);

// returns a new linker state object
s_linker_state* new_linker(char type, vector<string> obj_filenames, string output_filename, vector<pair<string, long>> section_place_locations);

// links all files, the relocatable option
s_error export_linked_file_rel(s_linker_state* linker_state);

// links all files, the hex option 
s_error export_linked_file_hex(s_linker_state* linker_state);

vector<s_object_file> get_object_files(s_linker_state* linker_state);

s_symbol_table* get_symbol_table_linker(s_linker_state* linker_state);

// returns the linked files, the relocatable option
string output_linked_to_string_rel(s_linker_state* linker_state);

// returns the linked files, the hex option
string output_linked_to_string_hex(s_linker_state* linker_state);

// returns nullptr if it doesnt exist
s_section* get_section_by_name_from_linked_file(s_linker_state* linker_state, string name);

// adds the section to the linked file, also updates the indexes of everything
void add_section_to_linked_file(s_linker_state* linker_state, s_section* section, s_symbol_table* old_st);