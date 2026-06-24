#pragma once
#include <string>
#include "elf.h"
#include "code.h"

using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

char* program_to_string(s_program *program);

#ifdef __cplusplus
}
#endif

string section_to_string(s_section *section);

string symbol_table_to_string(s_symbol_table *sym_table);

string rela_table_to_string(s_rela_table *rela_table);

#ifdef __cplusplus
extern "C" {
#endif

char* asm_instruction_name(e_asm_instruction inst);

char* asm_directive_name(e_asm_directive dir);

char* asm_line_to_string(s_asm_line *line);

#ifdef __cplusplus
}
#endif

string char_to_hex_string(char x);

string long_to_hex_string(long x);
