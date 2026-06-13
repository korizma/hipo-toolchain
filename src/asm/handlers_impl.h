#ifndef HANDLERS_IMPL_H
#define HANDLERS_IMPL_H

#define INSTRUCTION_BYTE_LEN 4
#define WORD_SIZE 4
#define SKIP_BYTE_LIMIT 2 << 20

#include "code.h"
#include "elf.h"
#include "error.h"

// instructions

s_error* handle_halt(s_asm_line* line, s_section* s);

s_error* handle_int(s_asm_line* line, s_section* s);

s_error* handle_iret(s_asm_line* line, s_section* s);

s_error* handle_call(s_asm_line* line, s_section* s);

s_error* handle_ret(s_asm_line* line, s_section* s);

s_error* handle_branch(s_asm_line* line, s_section* s);

s_error* handle_push(s_asm_line* line, s_section* s);

s_error* handle_pop(s_asm_line* line, s_section* s);

s_error* handle_xchg(s_asm_line* line, s_section* s);

s_error* handle_arthm(s_asm_line* line, s_section* s);

s_error* handle_not(s_asm_line* line, s_section* s);

s_error* handle_logic(s_asm_line* line, s_section* s);

s_error* handle_sh(s_asm_line* line, s_section* s);

s_error* handle_ld(s_asm_line* line, s_section* s);

s_error* handle_st(s_asm_line* line, s_section* s);

s_error* handle_control_rw(s_asm_line* line, s_section* s);

// directives

s_error* handle_global(s_asm_line* line, s_section* s);
s_error* handle_extern(s_asm_line* line, s_section* s);
s_error* handle_section(s_asm_line* line, s_section* s);
s_error* handle_word(s_asm_line* line, s_section* s);
s_error* handle_skip(s_asm_line* line, s_section* s);
s_error* handle_ascii(s_asm_line* line, s_section* s);
s_error* handle_equ(s_asm_line* line, s_section* s);
s_error* handle_end(s_asm_line* line, s_section* s);

s_error* handle_label(s_asm_line* line, s_section* s);

char* translate_to_binary(char oc, char mod, char reg_a, char reg_b, char reg_c, long disp);

char find_operation_code(e_asm_instruction instr);
char find_operation_mod(s_asm_line* line);

bool long_fit_in_12b(long a);

bool expr_is_invalid(s_expr* expr);

#endif