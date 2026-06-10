#include "section.h"
#include "stdlib.h"
#include "string.h"

program p;

section* new_section(char* name)
{
    section* s = (section*)malloc(sizeof(section));
    s->size = SECTION_START_SIZE;
    s->bytes = (char*)malloc(sizeof(char)*SECTION_START_SIZE);
    s->name = (char*)malloc(strlen(name) + 1);
    strcpy(s->name, name);
    s->next_free = 0;
    return s;
}

void write_bytes_to_section(section* s, char* bytes, int num)
{
    if (s->next_free + num > s->size)
    {
        s->size += SECTION_INCREMENT_SIZE;
        s->bytes = realloc(s->bytes, s->size * sizeof(char));
    }
    memcpy(s->bytes + sizeof(char)*s->next_free, bytes, num);
    s->next_free += num;
}

void init_program()
{
    p.sections = (section**)malloc(sizeof(section*)*PROGRAM_START_SECTION_SIZE);
    p.section_arr_size = PROGRAM_START_SECTION_SIZE;
    p.number_of_sections = 0;
}

void add_section_to_program(section* s)
{
    if (p.number_of_sections == p.section_arr_size)
    {
        p.section_arr_size += PROGRAM_SECTION_INCREMENT;
        p.sections = realloc(p.sections, sizeof(section*) * p.section_arr_size);
    }
    p.sections[p.number_of_sections++] = s;
}

void export_program_to_elf()
{
    
}
