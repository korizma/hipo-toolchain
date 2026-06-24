#ifndef ELF_H
#define ELF_H

#include <stdbool.h>
#include "section.h"
#include "sym_table.h"
#include "rela_table.h"

#define PROGRAM_START_SECTION_SIZE 10
#define PROGRAM_SECTION_INCREMENT 10

typedef struct s_program
{
    s_section** sections, *curr_section;
    int number_of_sections, section_arr_size;
    s_symbol_table* sym_table;

    bool has_ended;
} s_program;

#ifdef __cplusplus
extern "C" {
#endif

extern s_program p;

void init_program();

void add_section_to_program(s_section* s);

void export_program_to_binary(const char* filename);

void export_program_to_text(const char* filename);

void print_program();

typedef struct 
{
    long string_num, size;
    char** strings;
} s_string_table;

typedef struct s_final_output
{
    long pos, size;
    char* bytes;

    s_string_table str_table;
} s_final_output;

void add_string_to_string_table(s_final_output* output, char* string);

long get_index_of_string_in_string_table(s_final_output* output, char* string);

long add_and_get_string_string_table(s_final_output* output, char* string);

void write_n_bytes_final_output(s_final_output* output, char* bytes, int n);

// needs to free after
char* long_to_8_bytes(long x);
char* int_to_4_bytes(int x);

#ifdef __cplusplus
}
#endif

#endif
