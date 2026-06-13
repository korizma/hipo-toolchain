#include "error.h"
#include <stdlib.h>
#include <stdio.h>

static struct 
{
    const char* msg;
    e_error_type type;
} error_messages[] = 
{
    { "No error", ERR_NONE }
};

s_error* new_error(s_asm_line* line, e_error_type type)
{
    s_error* err = malloc(sizeof(s_error));
    err->e = type;
    err->line = line;
    return err;
}

void handle_error(s_error* err)
{
    const char* msg = error_messages[err->e].msg;
    printf("Error: %s\n", msg);
    char* line_str = asm_line_to_string(err->line);
    printf("Line %d: %s\n", err->line->line_num, line_str);
    free(line_str);
}