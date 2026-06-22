#ifndef SECTION_H
#define SECTION_H

#define SECTION_START_SIZE 16
#define SECTION_INCREMENT_SIZE 16

typedef struct s_rela_table s_rela_table;

typedef struct s_section
{
    char* name;
    unsigned long next_free, size;
    char* bytes;
    s_rela_table* rela_table;
} s_section;

s_section* new_section(char* name);

void write_bytes_to_section(s_section* s, char* bytes, int num);

void skip_bytes_in_section(s_section* s, int num);

void print_section(s_section* section);

#endif
