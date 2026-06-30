#pragma once

#include <string>
#include <vector>

struct s_section;
struct s_error;

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
void add_literal_trampoline_entry(s_section* section, long section_offset, long referent_offset, long literal);

// adds a symbol trampoline entry for the described section
void add_symbol_trampoline_entry(s_section* section, long section_offset, long referent_offset, long symbol_index);

// writes one trampoline entry, returns an error msg
s_error write_trampoline_entry(s_trampoline_entry* entry);

// writes all of the trampoline entries, returns all error msgs
vector<s_error> write_trampolines();

//gets the location of the trampoline if it exists already
long get_trampoline_location_if_exists_literal(long literal);
long get_trampoline_location_if_exists_symbol(long symbol_index);