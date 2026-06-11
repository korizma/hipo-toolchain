#ifndef HANDLER_H
#define HANDLER_H
#include "elf.h"
#include "code.h"

#define HANDLERS_START_SIZE 10
#define HANDLERS_INCREASE_SIZE 10

typedef void (*handler_f)(s_asm_line* , s_section*);

typedef enum {
    HANDLER_LABEL,
    HANDLER_INSTRUCTION,
    HANDLER_DIRECTIVE
} handler_kind;

typedef struct {
    handler_kind kind;
    asm_instruction instruction;
    asm_directive directive;
    handler_f handler;
} s_handler_entry;

typedef struct 
{
    s_handler_entry* entries;
    int next_avail, size;
} s_handlers_arr;

extern s_handlers_arr h_arr;

void init_handler_arr();

void register_label_handler(handler_f handler);

void register_instruction_handler(handler_f handler, asm_instruction instruction);

void register_directive_handler(handler_f handler, asm_directive directive);

void handle_line(s_asm_line* line);

#endif
