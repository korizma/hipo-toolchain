#ifndef HANDLERS_IMPL_H
#define HANDLERS_IMPL_H

#include "code.h"
#include "elf.h"

void handler_halt(s_asm_line* line, s_section* s);
void handler_int(s_asm_line* line, s_section* s);
void handler_iret(s_asm_line* line, s_section* s);
void handler_call(s_asm_line* line, s_section* s);
void handler_ret(s_asm_line* line, s_section* s);
void handler_jmp(s_asm_line* line, s_section* s);
void handler_beq(s_asm_line* line, s_section* s);
void handler_bne(s_asm_line* line, s_section* s);
void handler_bgt(s_asm_line* line, s_section* s);
void handler_push(s_asm_line* line, s_section* s);
void handler_pop(s_asm_line* line, s_section* s);
void handler_xchg(s_asm_line* line, s_section* s);
void handler_add(s_asm_line* line, s_section* s);
void handler_sub(s_asm_line* line, s_section* s);
void handler_mul(s_asm_line* line, s_section* s);
void handler_div(s_asm_line* line, s_section* s);
void handler_not(s_asm_line* line, s_section* s);
void handler_and(s_asm_line* line, s_section* s);
void handler_or(s_asm_line* line, s_section* s);
void handler_xor(s_asm_line* line, s_section* s);
void handler_shl(s_asm_line* line, s_section* s);
void handler_shr(s_asm_line* line, s_section* s);
void handler_ld(s_asm_line* line, s_section* s);
void handler_st(s_asm_line* line, s_section* s);
void handler_csrrd(s_asm_line* line, s_section* s);
void handler_csrwr(s_asm_line* line, s_section* s);

void handler_global(s_asm_line* line, s_section* s);
void handler_extern(s_asm_line* line, s_section* s);
void handler_s_section(s_asm_line* line, s_section* s);
void handler_word(s_asm_line* line, s_section* s);
void handler_skip(s_asm_line* line, s_section* s);
void handler_ascii(s_asm_line* line, s_section* s);
void handler_equ(s_asm_line* line, s_section* s);
void handler_end(s_asm_line* line, s_section* s);

void handler_label(s_asm_line* line, s_section* s);

char* translate_to_binary(char oc, char mod, char reg_a, char reg_b, char reg_c, long disp);

#endif
