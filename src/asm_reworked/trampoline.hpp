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

//  adds a literal trampoline entry for the described section
void add_literal_trampline_entry(s_section* section, long section_offset, long referent_offset, long literal);

// adds a symbol trampoline entry for the described section
void add_symbol_trampline_entry(s_section* section, long section_offset, long referent_offset, string symbol);

// writes one trampoline entry, returns an error msg
string write_trampoline_entry(s_trampoline_entry* entry);

// writes all of the trampoline entries, returns all error msgs
vector<string> write_trampolines();