#ifndef ASM_MAIN_H
#define ASM_MAIN_H

#include "code.h"

#define ASM_LINES_START_SIZE 10
#define ASM_LINES_INCREMENT 10

typedef struct
{
    s_asm_line** lines;
    int curr_line, size;
} s_asm_file;

void init_asm_file();

void assemble_file(const char* filename);

#endif