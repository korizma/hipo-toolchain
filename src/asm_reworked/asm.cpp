#include "asm.hpp"
#include <stdlib.h>
#include "symbol_table.hpp"
#include "asm_directive.hpp"
#include "asm_instruction.hpp"
#include "section.hpp"
#include <iostream>
#include <fstream>
#include "rela_table.hpp"

using namespace std;

s_program program;

s_program* get_program()
{
    return &program;
}

void init_program()
{
    program.symbol_table = (s_symbol_table*)malloc(sizeof(s_symbol_table));
}

void add_instruction_to_program(s_asm_instruction* instruction)
{
    s_asm_line new_line;
    new_line.asm_type = ASM_INSTRUCTION;
    new_line.instruction = instruction;

    program.lines.push_back(new_line);
}

void add_directive_to_program(s_asm_directive* directive)
{
    s_asm_line new_line;
    new_line.asm_type = ASM_DIRECTIVE;
    new_line.directive = directive;

    program.lines.push_back(new_line);
}

void add_label_to_program(string label)
{
    s_asm_line new_line;
    new_line.asm_type = ASM_LABEL;
    new_line.label = label;

    program.lines.push_back(new_line);
}

vector<s_asm_line> get_program_lines()
{
    return program.lines;
}

bool assemble_program_to_file(string filename)
{
    init_program();
    assemble_lines();

    std::ofstream file(filename);

    if (!file.is_open()) {
        return false;
    }

    string content = program_to_string();

    file << content;
    return true;
}

bool assemble_lines()
{
    vector<s_asm_line> lines = get_program_lines();

    bool has_errors = false;
    for (s_asm_line line : lines)
    {
        string error;
        if (line.asm_type == ASM_LABEL)
            error = handle_label(&line);

        else if (line.asm_type == ASM_INSTRUCTION)
            error = handle_asm_instruction(line.instruction);

        else if (line.asm_type == ASM_DIRECTIVE)
            error = handle_asm_directive(line.directive);

        if (error != "")
        {
            has_errors = true;
            cout << error << endl;
        }
    }

    return !has_errors;
}

string program_to_string()
{
    string program_string = "";

    program_string += symbol_table_to_string();

    for (s_section section : program.section_list)
    {
        program_string += section_to_string(&section);
        if (section.has_rela)
            program_string += rela_table_to_string(section.rela_table);
    }

    return program_string;
}