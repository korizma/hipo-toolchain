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

    bool has_ended;
} s_program;

extern s_program p;

void init_program();

void add_section_to_program(s_section* s);

void export_program_to_elf(const char* filename);

void print_program();

typedef struct 
{
    char* bytes;
    int num_written, size;
} s_elf_final;

typedef struct 
{
    long string_num, size;
    char** strings;
} s_string_table;


#endif
