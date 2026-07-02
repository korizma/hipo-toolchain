#pragma once

#include <vector>
#include <string>

using namespace std;

#include "section.hpp"
#include "symbol_table.hpp"

typedef struct s_object_file
{
    vector<s_section> sections;
    s_symbol_table symbol_table;
} s_object_file;

// loads the file, and adds all section names to the symbol table
s_object_file* load_object_file(s_linker_state* state, string filename);



