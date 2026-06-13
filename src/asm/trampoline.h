#ifndef TRAMPOLINE_H
#define TRAMPOLINE_H

#include "section.h"
#include "code.h"
#include "error.h"

#define TRAMPOLINE_START_SIZE 10
#define TRAMPOLINE_INCREMENT 10

#define TRAMPOLINE_ONE_ENTRY_MEM_SIZE 4

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

    // after its done
    bool is_done;
    long trampoline_location;
} s_trampoline_entry;

typedef struct 
{
    s_trampoline_entry** entries;
    int entry_num, size;
} s_trampoline;

void init_trampoline();

void free_trampoline();

void add_trampoline_entry(s_section* s, s_asm_line* line, long literal, char* symbol, e_trampoline_entry_type type);

int write_trampolines();

void print_trampoline();

s_error* write_trampoline_literal(s_trampoline_entry* entry);

s_error* write_trampoline_symbol(s_trampoline_entry* entry);

// if they use the same symbol or literal and are in the same
s_trampoline_entry* find_matching_trampoline_entry(s_trampoline_entry* entry);

s_error* write_displacement_to_line(s_asm_line* line, long trampoline_location);

#endif 