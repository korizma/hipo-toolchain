#ifndef HANDLERS_IMPL_H
#define HANDLERS_IMPL_H

#include "code.h"
#include "elf.h"

void handle_halt(s_asm_line* line, s_section* s);
void handle_int(s_asm_line* line, s_section* s);
void handle_iret(s_asm_line* line, s_section* s);
void handle_call(s_asm_line* line, s_section* s);
void handle_ret(s_asm_line* line, s_section* s);
void handle_jmp(s_asm_line* line, s_section* s);
void handle_beq(s_asm_line* line, s_section* s);
void handle_bne(s_asm_line* line, s_section* s);
void handle_bgt(s_asm_line* line, s_section* s);
void handle_push(s_asm_line* line, s_section* s);
void handle_pop(s_asm_line* line, s_section* s);
void handle_xchg(s_asm_line* line, s_section* s);
void handle_add(s_asm_line* line, s_section* s);
void handle_sub(s_asm_line* line, s_section* s);
void handle_mul(s_asm_line* line, s_section* s);
void handle_div(s_asm_line* line, s_section* s);
void handle_not(s_asm_line* line, s_section* s);
void handle_and(s_asm_line* line, s_section* s);
void handle_or(s_asm_line* line, s_section* s);
void handle_xor(s_asm_line* line, s_section* s);
void handle_shl(s_asm_line* line, s_section* s);
void handle_shr(s_asm_line* line, s_section* s);
void handle_ld(s_asm_line* line, s_section* s);
void handle_st(s_asm_line* line, s_section* s);
void handle_csrrd(s_asm_line* line, s_section* s);
void handle_csrwr(s_asm_line* line, s_section* s);

void handle_global(s_asm_line* line, s_section* s);
void handle_extern(s_asm_line* line, s_section* s);
void handle_s_section(s_asm_line* line, s_section* s);
void handle_word(s_asm_line* line, s_section* s);
void handle_skip(s_asm_line* line, s_section* s);
void handle_ascii(s_asm_line* line, s_section* s);
void handle_equ(s_asm_line* line, s_section* s);
void handle_end(s_asm_line* line, s_section* s);

void handle_label(s_asm_line* line, s_section* s);

char* translate_to_binary(char oc, char mod, char reg_a, char reg_b, char reg_c, long disp);

bool expr_is_valid(s_expr* expression, s_section* curr_section);
long calculate_expression(s_expr* expression);

bool label_fits_in_12b(char* label, s_section* curr_section);
long label_offset(char* label, s_section* curr_section);



#endif
