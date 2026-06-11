#ifndef HANDLERS_IMPL_H
#define HANDLERS_IMPL_H

#define INSTRUCTION_BYTE_LEN 4

#include "code.h"
#include "elf.h"

// instructions

int handle_halt(s_asm_line* line, s_section* s);

int handle_int(s_asm_line* line, s_section* s);

int handle_iret(s_asm_line* line, s_section* s);

int handle_call(s_asm_line* line, s_section* s);

int handle_ret(s_asm_line* line, s_section* s);

int handle_branch(s_asm_line* line, s_section* s);

int handle_push(s_asm_line* line, s_section* s);

int handle_pop(s_asm_line* line, s_section* s);

int handle_xchg(s_asm_line* line, s_section* s);

int handle_arthm(s_asm_line* line, s_section* s);

int handle_not(s_asm_line* line, s_section* s);

int handle_logic(s_asm_line* line, s_section* s);

int handle_sh(s_asm_line* line, s_section* s);

int handle_ld(s_asm_line* line, s_section* s);

int handle_st(s_asm_line* line, s_section* s);

int handle_control_rw(s_asm_line* line, s_section* s);

// directives

int handle_global(s_asm_line* line, s_section* s);
int handle_extern(s_asm_line* line, s_section* s);
int handle_section(s_asm_line* line, s_section* s);
int handle_word(s_asm_line* line, s_section* s);
int handle_skip(s_asm_line* line, s_section* s);
int handle_ascii(s_asm_line* line, s_section* s);
int handle_equ(s_asm_line* line, s_section* s);
int handle_end(s_asm_line* line, s_section* s);

int handle_label(s_asm_line* line, s_section* s);

char* translate_to_binary(char oc, char mod, char reg_a, char reg_b, char reg_c, long disp);

bool expr_is_valid(s_expr* expression, s_section* curr_section);
long calculate_expression(s_expr* expression);


char find_operation_code(e_asm_instruction instr);
char find_operation_mod(s_asm_line* line);

#endif
