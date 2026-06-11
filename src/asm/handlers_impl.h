#ifndef HANDLERS_IMPL_H
#define HANDLERS_IMPL_H

#define INSTRUCTION_BYTE_LEN 4

#include "code.h"
#include "elf.h"

int handle_halt(s_asm_line* line, s_section* s);
int handle_int(s_asm_line* line, s_section* s);
int handle_iret(s_asm_line* line, s_section* s);
int handle_call(s_asm_line* line, s_section* s);
int handle_ret(s_asm_line* line, s_section* s);
int handle_jmp(s_asm_line* line, s_section* s);
int handle_beq(s_asm_line* line, s_section* s);
int handle_bne(s_asm_line* line, s_section* s);
int handle_bgt(s_asm_line* line, s_section* s);
int handle_push(s_asm_line* line, s_section* s);
int handle_pop(s_asm_line* line, s_section* s);
int handle_xchg(s_asm_line* line, s_section* s);

int handle_arthm(s_asm_line* line, s_section* s);

int handle_not(s_asm_line* line, s_section* s);

int handle_logic(s_asm_line* line, s_section* s);

int handle_sh(s_asm_line* line, s_section* s);

int handle_ld(s_asm_line* line, s_section* s);
int handle_st(s_asm_line* line, s_section* s);
int handle_csrrd(s_asm_line* line, s_section* s);
int handle_csrwr(s_asm_line* line, s_section* s);

int handle_global(s_asm_line* line, s_section* s);
int handle_extern(s_asm_line* line, s_section* s);
int handle_s_section(s_asm_line* line, s_section* s);
int handle_word(s_asm_line* line, s_section* s);
int handle_skip(s_asm_line* line, s_section* s);
int handle_ascii(s_asm_line* line, s_section* s);
int handle_equ(s_asm_line* line, s_section* s);
int handle_end(s_asm_line* line, s_section* s);

int handle_label(s_asm_line* line, s_section* s);

char* translate_to_binary(char oc, char mod, char reg_a, char reg_b, char reg_c, long disp);

bool expr_is_valid(s_expr* expression, s_section* curr_section);
long calculate_expression(s_expr* expression);

bool label_fits_in_12b(char* label, s_section* curr_section);
long label_offset(char* label, s_section* curr_section);

char find_operation_code(e_asm_instruction instr);
char find_operation_mod(s_asm_line* line);

#endif
