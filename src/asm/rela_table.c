#include "rela_table.h"
#include "section.h"
#include <stdlib.h>

void create_rela_table(s_section* s)
{
    s->rela_table = (s_rela_table*)malloc(sizeof(s_rela_table));
    s->rela_table->size = RELA_TABLE_START_SIZE;
    s->rela_table->entry_num = 0;
    s->rela_table->entries = (s_Elf64_Rela_entry**)malloc(sizeof(s_Elf64_Rela_entry*) * s->rela_table->size);
    s->rela_table->section = s;
}
