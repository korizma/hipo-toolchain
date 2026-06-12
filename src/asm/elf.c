#include "elf.h"
#include <stdlib.h>
#include <string.h>

s_program p;

void init_program()
{
    p.sections = (s_section**)malloc(sizeof(s_section*)*PROGRAM_START_SECTION_SIZE);
    p.section_arr_size = PROGRAM_START_SECTION_SIZE;
    p.number_of_sections = 0;

    p.sym_table = (s_symbol_table*)malloc(sizeof(s_symbol_table));
    p.sym_table->symbol_num = 0;
    p.sym_table->size = SYM_TABLE_START_SIZE;
    p.sym_table->symbols = (s_Elf64_Sym**)malloc(sizeof(s_Elf64_Sym*) * p.sym_table->size);
}

void add_section_to_program(s_section* s)
{
    if (p.number_of_sections == p.section_arr_size)
    {
        p.section_arr_size += PROGRAM_SECTION_INCREMENT;
        p.sections = realloc(p.sections, sizeof(s_section*) * p.section_arr_size);
    }
    p.sections[p.number_of_sections++] = s;
}

int find_section_index(char* name)
{
    for (int i = 0; i < p.number_of_sections; i++)
    {
        if (strcmp(p.sections[i]->name, name) == 0)
            return i;
    }
    return -1;
}


void export_program_to_elf()
{
    
}
