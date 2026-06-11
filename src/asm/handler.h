#ifndef HANDLER_H
#define HANDLER_H
#include "elf.h"
#include "code.h"

#define HANDLERS_START_SIZE 10
#define HANDLERS_INCREASE_SIZE 10

typedef void (*handler_f)(asm_line* , s_section*);

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
} handler_entry;

typedef struct 
{
    handler_entry* entries;
    int next_avail, size;
} handlers_arr;

extern handlers_arr h_arr;

void init_handler_arr();

void register_label_handler(handler_f handler);

void register_instruction_handler(handler_f handler, asm_instruction instruction);

void register_directive_handler(handler_f handler, asm_directive directive);

void handle_line(asm_line* line);

#endif
