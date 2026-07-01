#pragma once

#include <string>
#include <vector>

using namespace std;

struct s_section; 
struct s_symbol_table_entry;
struct s_symbol_table;
struct s_program;
struct s_linker_state;

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
s_rela_table_entry* create_new_rela_table_entry(s_section* section, long addend, long offset, char relocation_type, long symbol_index);

// returns the string representation of the rela table, used for the output text file
string rela_table_to_string(s_symbol_table* symbol_table, s_rela_table* rela_table);

// signature not final yet
s_rela_table* import_rela_table(vector<string> lines, s_symbol_table* symbol_table, vector<s_section>& section_list);

// executes all relocations for this symbol
void rela_table_symbol_execute_and_remove(s_symbol_table* symbol_table, vector<s_section>& section_list, s_symbol_table_entry* symbol);

// updates the entries for the symbol
void rela_table_symbol_update(s_symbol_table* symbol_table, vector<s_section>& section_list, s_symbol_table_entry* symbol);

// writes the relocation
void write_rela_table_entry(s_symbol_table* symbol_table, s_rela_table_entry* entry, s_section* section);

// linker helpers

// updates the rela table for the section so each offset is increased by increase, also so they have correct indexes
void update_rela_table_linker(s_linker_state* linker_state, s_section* s1, s_symbol_table* st1, long increase);