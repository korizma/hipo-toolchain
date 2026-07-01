#include "object_file.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include "misc.hpp"
#include "rela_table.hpp"


using namespace std;


string read_all_text(const string& filename)
{
    ifstream file(filename);
    if (!file)
        return "";

    std::ostringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// loads the file, and adds all section names to the symbol table
s_object_file* load_object_file(s_linker_state* state, string filename)
{
    string text = read_all_text(filename);
    if (text == "")
        return nullptr;

    s_object_file* obj_file = new s_object_file();

    vector<string> lines = split_string(text, '\n');
    vector<string> curr;

    bool sym_table_parsed = false;
    
    for (string& line : lines)
    {
        if (curr.size() != 0 && line[0] == '#')
        {
            if (!sym_table_parsed)
            {
                obj_file->symbol_table = *import_symbol_table(curr);
                sym_table_parsed = true;
            }
            else
            {
                s_rela_table* rela = import_rela_table(curr, &obj_file->symbol_table, obj_file->sections);

                if (rela == NULL)
                {
                    s_section* curr_section = import_section(curr, &obj_file->symbol_table);
                    obj_file->sections.push_back(*curr_section);
                }
            }
            curr.clear();
        }

        curr.push_back(line);
    }
    if (!sym_table_parsed)
    {
        obj_file->symbol_table = *import_symbol_table(curr);
        sym_table_parsed = true;
    }
    else
    {
        s_rela_table* rela = import_rela_table(curr, &obj_file->symbol_table, obj_file->sections);

        if (rela == NULL)
        {
            s_section* curr_section = import_section(curr, &obj_file->symbol_table);
            obj_file->sections.push_back(*curr_section);
        }
    }
    curr.clear();

    return obj_file;
}

