#pragma once

#include <string>
#include <vector>

using namespace std;

struct s_section;
struct s_symbol_table;
struct s_asm_instruction;
struct s_asm_directive;
struct s_trampoline;

#define ASM_INSTRUCTION 0
#define ASM_DIRECTIVE 1
#define ASM_LABEL 2

typedef struct s_asm_line
{
    char asm_type;
    s_asm_instruction* instruction;
    s_asm_directive* directive;
    string label;
} s_asm_line;

typedef struct s_program
{
    // from parser
    vector<s_asm_line> lines;
    bool lines_ended;

    // symbol table
    s_symbol_table* symbol_table;

    // section list
    vector<s_section> section_list;

    // trampoline list
    s_trampoline* trampoline;

} s_program;

// initializes the programs variables
void init_program();

// frees all heap used memory
void free_program();

// returns the program as a string, used for the output text file
string program_to_string();


// adds an instruction to the lines
void add_instruction_to_program(s_asm_instruction* instruction);
// adds a directive to the lines
void add_directive_to_program(s_asm_directive* directive);
// adds a label declaration to the lines
void add_label_to_program(string label);

// assembles the program and outputs to the mentioned filename
bool assemble_program_to_file(string filename);
// helper function to assemble everything before exporting
bool assemble_lines();


// handler for asm lines labels
string handle_label(s_asm_line* line);


// adds a section to a program
void add_section_to_program(s_section* section);


// returns the main program struct
s_program* get_program();
// returns all program lines
vector<s_asm_line> get_program_lines();
// returns the symbol table
s_symbol_table* get_symbol_table();
// returns the trampoline entries
s_trampoline* get_trampoline();