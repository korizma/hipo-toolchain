#pragma once

#include <string>
#include <vector>

struct s_section;

using namespace std;

#define TRMP_SYM 0
#define TRMP_LIT 1

typedef struct s_trampoline_entry
{
    s_section* section;
    
    long section_offset;    // which 2 bytes to write 12 bits
    long referent_offset;   // offset from which to calcualte 

    char type;
    long literal;
    long symbol_symbol_table_index;

    // after writing
    long trampoline_location;
} s_trampoline_entry;

typedef struct s_trampoline
{
    vector<s_trampoline_entry> entries;
} s_trampoline;

void add_literal_trampline_entry(s_section* section, long section_offset, long referent_offset, long literal);

void add_symbol_trampline_entry(s_section* section, long section_offset, long referent_offset, string symbol);

string write_trampoline_entry(s_trampoline_entry* entry);

string write_trampolines();