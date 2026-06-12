#include "asm_main.h"
#include <stdlib.h>
#include "handler.h"
#include "elf.h"
#include "trampoline.h"

s_asm_file asm_file;

void init_asm_file()
{
    asm_file.lines = (s_asm_line**)malloc(ASM_LINES_START_SIZE * sizeof(s_asm_line*));
    asm_file.curr_line = 0;
    asm_file.size = ASM_LINES_START_SIZE;

    init_handler_arr();
    register_init_handlers();
    init_program();
    init_trampoline();
}

void assemble_file(const char* filename)
{
    for (int i = 0; i < asm_file.curr_line; i++)
    {
        handle_line(asm_file.lines[i]);
    }

    print_program();
    print_trampoline();
}