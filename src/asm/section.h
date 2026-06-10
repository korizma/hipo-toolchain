#ifndef SECTION_H
#define SECTION_H

#define SECTION_START_SIZE 16
#define SECTION_INCREMENT_SIZE 16
#define PROGRAM_START_SECTION_SIZE 10
#define PROGRAM_SECTION_INCREMENT 10

typedef struct
{
    char* name;
    int next_free, size;
    char* bytes;
} section;

typedef struct
{
    section** sections;
    int number_of_sections, section_arr_size;
} program;

extern program p;

section* new_section(char* name);

void write_bytes_to_section(section* s, char* bytes, int num);

void init_program();

void add_section_to_program(section* s);

void export_program_to_elf();

#endif
