#ifndef HANDLER_H
#define HANDLER_H
#include "elf.h"
#include "code.h"

#define HANDLERS_START_SIZE 10
#define HANDLERS_INCREASE_SIZE 10

typedef int (*handler_f)(s_asm_line* , s_section*);

typedef enum {
    HANDLER_LABEL,
    HANDLER_INSTRUCTION,
    HANDLER_DIRECTIVE
} e_handler_kind;

typedef struct {
    e_handler_kind kind;
    e_asm_instruction instruction;
    e_asm_directive directive;
    handler_f handler;
} s_handler_entry;

typedef struct 
{
    s_handler_entry** entries;
    int next_avail, size;
} s_handlers_arr;

extern s_handlers_arr h_arr;

void init_handler_arr();

void register_handler(e_handler_kind kind, e_asm_instruction inst, e_asm_directive direct, handler_f handler);

void register_handler_struct(s_handler_entry* entry);

void handle_line(s_asm_line* line);

void register_init_handlers();

#endif
