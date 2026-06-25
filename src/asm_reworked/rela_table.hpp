#pragma once

#include <string>
#include <vector>

using namespace std;

struct s_section; 

#define R_HIPO_32 0
#define R_HIPO_12 1

typedef struct s_rela_table_entry
{
    long addend;
    long symbol_symbol_table_index;
    char relocation_type;
    long offset_in_section;
} s_rela_table_entry;

typedef struct s_rela_table
{
    vector<s_rela_table_entry> entries;
    long section_symbol_table_index;
} s_rela_table;

// helpers

// creates new rela table for section, if it already exists returns the rela table
s_rela_table* new_rela_table(s_section* section);

// returns the rela_table for section, if it doesnt exists return 0
s_rela_table* get_rela_table(s_section* section);

// creates a new entry for in the rela table for the given section, if the rela table doesnt exists it creates it 
s_rela_table_entry* create_new_rela_table_entry(s_section* section, long addend, long offset, char relocation_type);

// returns the string representation of the rela table, used for the output text file
string rela_table_to_string(s_rela_table* rela_table);

// signature not final yet
s_rela_table* import_rela_table(int i_dont_know_yet);
