#include "asm_main.h"
#include <stdlib.h>
#include "handler.h"
#include "elf.h"
#include "trampoline.h"
#include <stdio.h>

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
    bool has_error = false;
    for (int i = 0; i < asm_file.curr_line; i++)
    {
        s_error* err = handle_line(asm_file.lines[i]);
        if (err != 0)
        {
            print_error(err);
            free_error(err);
            has_error = true;
        }
        if (p.has_ended)
            break;
    }

    if (p.curr_section != 0)
    {
        update_section_size_in_sym_table(p.curr_section);
        p.curr_section = 0;
    }

    s_error* err = finalize_symbol_table();
    if (err != 0)
    {
        print_error(err);
        free_error(err);
        has_error = true;
    }

    int trampoline_status = write_trampolines();

    if (has_error || trampoline_status != 0)
        printf("Assembly failed due to errors!\n");
    else
        printf("Assembly successful!\n");


    for (int i = 0; i < p.number_of_sections; i++)
    {
        check_rela_table(p.sections[i]->rela_table);
    }

    print_program();
    print_trampoline();


    free_trampoline();
}
