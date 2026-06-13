#ifndef ERROR_H
#define ERROR_H

#include "code.h"

typedef enum 
{
    ERR_NONE = 0
} e_error_type;

typedef struct 
{
    e_error_type e;
    s_asm_line* line;
} s_error;

s_error* new_error(s_asm_line* line, e_error_type type);

void handle_error(s_error* err);

#endif 