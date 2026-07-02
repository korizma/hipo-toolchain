#pragma once

#include <string>
#include <vector>
#include "error.hpp"

using namespace std;

struct s_rela_table; 
struct s_program;
struct s_symbol_table;
struct s_linker_state;

typedef struct s_section
{
    long sym_table_index;
    vector<char> bytes;
    bool has_rela;
    s_rela_table* rela_table;

    // linker
    long placed_at;
    bool placed;
} s_section;

// helpers

// allocates and creates new section
s_section* new_section();

// returns the name/symbol of the section form the symbol table
string get_section_symbol(s_symbol_table* symbol_table, s_section* section);

// writes bytes to the section
void write_bytes_to_section(s_section* section, vector<char> bytes);

// writes N 0 bytes to the section
void skip_bytes_in_section(s_section* section, long num_bytes);

// binds a relocation table to the seciton
void bind_rela_table_to_section(s_section* section, s_rela_table* rela_table);

// returns the string view of the section, used for final text file export
string section_to_string(s_symbol_table *symbol_table, s_section* section);

// returns the section offset
long get_section_offset(s_section* section);

s_section* import_section(vector<string> lines, s_symbol_table* symbol_table);

s_section* find_section_by_name(vector<s_section>& section_list, s_symbol_table* symbol_table, string name);

// linker helpers

void combine_all_sections(s_linker_state* linker_state);

// the result will be in the linked object file
void combine_section_to_linked(s_linker_state* linker_state, s_section* s1, s_symbol_table* st1);

s_error place_all_sections(s_linker_state* linker_state);

string section_to_linked_string(s_section* section);