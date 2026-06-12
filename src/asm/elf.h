#ifndef ELF_H
#define ELF_H

#include <stdbool.h>
#include "section.h"
#include "sym_table.h"
#include "rela_table.h"

#define PROGRAM_START_SECTION_SIZE 10
#define PROGRAM_SECTION_INCREMENT 10

typedef struct
{
    s_section** sections, *curr_section;
    int number_of_sections, section_arr_size;
    s_symbol_table* sym_table;
} s_program;

extern s_program p;

int find_section_index(char* name);

void init_program();

void add_section_to_program(s_section* s);

void export_program_to_elf();

void print_program();

#endif
