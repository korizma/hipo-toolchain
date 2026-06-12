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

void create_rela_entry(s_section* s, unsigned long offset, int sym_index, e_Elf64_reloc_type type, long addend)
{
    if (s->rela_table == 0)
    {
        create_rela_table(s);
    }

    if (s->rela_table->entry_num == s->rela_table->entry_num)
    {
        s->rela_table->size += RELA_TABLE_INCREMENT;
        s->rela_table->entries = realloc(s->rela_table->entries, sizeof(s_Elf64_Rela_entry*) * s->rela_table->size);
    }

    s_Elf64_Rela_entry* new_entry = (s_Elf64_Rela_entry*)malloc(sizeof(s_Elf64_Rela_entry));
    new_entry->r_addend = addend;
    new_entry->sym_index = sym_index;
    new_entry->r_offset = offset;
    new_entry->reloc_type = type;

    s->rela_table->entries[s->rela_table->entry_num++] = new_entry;
}

