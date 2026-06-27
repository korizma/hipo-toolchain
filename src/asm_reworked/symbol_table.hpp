#pragma once

#include <string>
#include <vector>

using namespace std;

struct s_expr;
struct s_error;

// symbol table types
#define STT_SECTION 0
#define STT_NOTYPE  1

// symbol table bindings
#define STB_LOCAL 0
#define STB_GLOBAL 1

// symbol table state
#define STS_COMPLETE 0
#define STS_REFERENCE 1
#define STS_GLOBAL_REFERENCE 2
#define STS_EQU 3

typedef struct s_symbol_table_entry
{
    string name;
    char type;
    char binding;
    long section_symbol_table_index;
    long offset_or_value;   // during assembly this is the offset, after linking this is value
    long size;              // only for sections

    // assembly specific fields
    char state;

    // equ specific field
    s_expr* expression;
} s_symbol_table_entry;


typedef struct s_symbol_table
{
    vector<s_symbol_table_entry> entries;
} s_symbol_table;

// returns the s_symbol_table_entry pointer, if not found returns 0
s_symbol_table_entry* get_symbol_entry_index(int index);

// returns the s_symbol_table_entry pointer, if not found returns 0
s_symbol_table_entry* get_symbol_entry_symbol(string symbol);

// returns the s_symbol_table_entry pointer, if symbol conflict, returns 0
s_symbol_table_entry* create_new_symbol_entry(string symbol);

// returns the symbol pointer if it already exists, if not creates it and returns it
s_symbol_table_entry* get_and_create_new_symbol_entry(string symbol);

// return the entry in binary format
vector<char> symbol_table_entry_to_bytes(s_symbol_table_entry entry);

// returns the string view of the symbol table, used for final text file export
string symbol_table_to_string();

// signature not final yet
s_symbol_table* import_symbol_table(int i_dont_know_yet);

// finalizes each equ symbol, and looks for undefined symbols
vector<s_error> finalize_symbol_table();

