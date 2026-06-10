#ifndef HANDLER_H
#define HANDLER_H
#include "section.h"
#include "code.h"

#define HANDLERS_START_SIZE 10
#define HANDLERS_INCREASE_SIZE 10

typedef void (*handler_f)(asm_line* , section*);

typedef struct 
{
    handler_f* handlers;
    char** names;
    int next_avail, size;
} handlers_arr;

extern handlers_arr h_arr;

void init_handler_arr();

void register_handler(handler_f handler, char* name);

void handle_line(asm_line* line);

#endif
