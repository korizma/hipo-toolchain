#pragma once

#include <string>
#include <vector>

using namespace std;

struct s_rela_table; 

typedef struct s_section
{
    long sym_table_index;
    vector<char> bytes;
    bool has_rela;
    s_rela_table* rela_table;
} s_section;

// helpers

// allocates and creates new section
s_section* new_section();

// returns the name/symbol of the section form the symbol table
string get_section_symbol(s_section* section);

// writes bytes to the section
void write_bytes_to_section(s_section* section, vector<char> bytes);

// writes N 0 bytes to the section
void skip_bytes_in_section(s_section* section, long num_bytes);

// binds a relocation table to the seciton
void bind_rela_table_to_section(s_section* section, s_rela_table* rela_table);

// returns the string view of the section, used for final text file export
string section_to_string(s_section* section);

// returns the section offset
long get_section_offset(s_section* section);

// signature not final yet
s_section* import_section(int i_dont_know_yet);