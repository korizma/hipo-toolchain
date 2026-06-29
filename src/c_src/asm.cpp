#include "asm.hpp"
#include <stdlib.h>
#include "symbol_table.hpp"
#include "asm_directive.hpp"
#include "asm_instruction.hpp"
#include "section.hpp"
#include <iostream>
#include <fstream>
#include "rela_table.hpp"
#include "error.hpp"
#include "trampoline.hpp"

using namespace std;

s_program program;

s_program* get_program()
{
    return &program;
}

void init_program()
{
    program.lines.clear();
    program.lines_ended = false;
    program.section_list.clear();
    program.lines.reserve(4096);
    program.section_list.reserve(128);
    program.symbol_table = new s_symbol_table();
    program.trampoline = new s_trampoline();
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

string asm_line_to_string(s_asm_line* line)
{
    if (line == NULL)
        return "";

    if (line->asm_type == ASM_LABEL)
        return line->label + ":";
    if (line->asm_type == ASM_INSTRUCTION)
        return asm_instruction_to_string(line->instruction);
    if (line->asm_type == ASM_DIRECTIVE)
        return asm_directive_to_string(line->directive);

    return "";
}

vector<s_asm_line> get_program_lines()
{
    return program.lines;
}

bool assemble_program_to_file(string filename)
{
    bool success = assemble_lines();

    if (!success)
        return false;

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
        s_error error;
        if (line.asm_type == ASM_LABEL)
            error = handle_label(&line);

        else if (line.asm_type == ASM_INSTRUCTION)
            error = handle_asm_instruction(line.instruction);

        else if (line.asm_type == ASM_DIRECTIVE)
            error = handle_asm_directive(line.directive);

        if (!error.no_error)
        {
            has_errors = true;
            cout << error_to_string(error) << endl;
        }
    }

    vector<s_error> symbol_table_errors = finalize_symbol_table();

    for (s_error error : symbol_table_errors)
    {
        has_errors = true;
        cout << error_to_string(error) << endl;
    }

    vector<s_error> trampoline_errors = write_trampolines();

    for (s_error error : trampoline_errors)
    {
        has_errors = true;
        cout << error_to_string(error) << endl;
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


s_symbol_table* get_symbol_table()
{
    return program.symbol_table;
}

s_trampoline* get_trampoline()
{
    return program.trampoline;
}

s_section* get_current_section()
{
    if (program.section_list.empty())
        return NULL;
    return &program.section_list.back();
}

s_error handle_label(s_asm_line* line)
{
    s_section* curr_section = get_current_section();
    if (curr_section == NULL)
        return new_error(ERR_LINE_OUTSIDE_SECTION, asm_line_to_string(line));

    s_symbol_table_entry* symbol = get_symbol_entry_symbol(line->label);

    if (symbol != 0 && (symbol->state == STS_COMPLETE || symbol->state == STS_EQU))
        return new_error(ERR_DOUBLE_SYM_DECL, line->label);

    if (symbol != 0)
    {
        symbol->offset_or_value = curr_section->bytes.size();
        symbol->size = 0;
        symbol->type = STT_NOTYPE;
        symbol->state = STS_COMPLETE;
        symbol->section_symbol_table_index = curr_section->sym_table_index;
        if (symbol->binding != STB_GLOBAL)
            symbol->binding = STB_LOCAL;
    }
    else
    {
        symbol = create_new_symbol_entry(line->label);
        symbol->offset_or_value = curr_section->bytes.size();
        symbol->size = 0;
        symbol->type = STT_NOTYPE;
        symbol->state = STS_COMPLETE;
        symbol->binding = STB_LOCAL;
        symbol->section_symbol_table_index = curr_section->sym_table_index;
    }

    return new_no_error();
}

void free_program()
{
    delete program.trampoline;
    delete program.symbol_table;
    program.trampoline = NULL;
    program.symbol_table = NULL;
}

void add_section_to_program(s_section* section)
{
    program.section_list.push_back(*section);
}
