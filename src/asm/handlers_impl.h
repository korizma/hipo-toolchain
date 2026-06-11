#ifndef HANDLERS_IMPL_H
#define HANDLERS_IMPL_H

#include "code.h"
#include "elf.h"

void handler_halt(asm_line* line, s_section* s);
void handler_int(asm_line* line, s_section* s);
void handler_iret(asm_line* line, s_section* s);
void handler_call(asm_line* line, s_section* s);
void handler_ret(asm_line* line, s_section* s);
void handler_jmp(asm_line* line, s_section* s);
void handler_beq(asm_line* line, s_section* s);
void handler_bne(asm_line* line, s_section* s);
void handler_bgt(asm_line* line, s_section* s);
void handler_push(asm_line* line, s_section* s);
void handler_pop(asm_line* line, s_section* s);
void handler_xchg(asm_line* line, s_section* s);
void handler_add(asm_line* line, s_section* s);
void handler_sub(asm_line* line, s_section* s);
void handler_mul(asm_line* line, s_section* s);
void handler_div(asm_line* line, s_section* s);
void handler_not(asm_line* line, s_section* s);
void handler_and(asm_line* line, s_section* s);
void handler_or(asm_line* line, s_section* s);
void handler_xor(asm_line* line, s_section* s);
void handler_shl(asm_line* line, s_section* s);
void handler_shr(asm_line* line, s_section* s);
void handler_ld(asm_line* line, s_section* s);
void handler_st(asm_line* line, s_section* s);
void handler_csrrd(asm_line* line, s_section* s);
void handler_csrwr(asm_line* line, s_section* s);

void handler_global(asm_line* line, s_section* s);
void handler_extern(asm_line* line, s_section* s);
void handler_s_section(asm_line* line, s_section* s);
void handler_word(asm_line* line, s_section* s);
void handler_skip(asm_line* line, s_section* s);
void handler_ascii(asm_line* line, s_section* s);
void handler_equ(asm_line* line, s_section* s);
void handler_end(asm_line* line, s_section* s);

void handler_label(asm_line* line, s_section* s);

char* translate_to_binary(char oc, char mod, char reg_a, char reg_b, char reg_c, long disp);

#endif
