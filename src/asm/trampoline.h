#ifndef TRAMPOLINE_H
#define TRAMPOLINE_H

#include "section.h"
#include "code.h"

#define TRAMPOLINE_START_SIZE 10
#define TRAMPOLINE_INCREMENT 10

typedef enum
{
    TE_JUMP_LITERAL,
    TE_JUMP_SYMBOL,
    TE_LD_IMM_LITERAL,
    TE_LD_IMM_SYMBOL,
    TE_ST_MEM_LITERAL,
    TE_ST_MEM_SYMBOL
} e_trampoline_entry_type;

typedef struct 
{
    s_section* section;
    long literal;
    char* symbol;
    s_asm_line* line;

    e_trampoline_entry_type type;
} s_trampoline_entry;

typedef struct 
{
    s_trampoline_entry** entries;
    int entry_num, size;
} s_trampoline;

void init_trampoline();

void add_trampoline_entry(s_section* s, s_asm_line* line, long literal, char* symbol, e_trampoline_entry_type type);

void write_trampolines();

void print_trampoline();

#endif 