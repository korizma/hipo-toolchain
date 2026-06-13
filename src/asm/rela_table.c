#include "rela_table.h"
#include "section.h"
#include "sym_table.h"
#include "elf.h"
#include <stdio.h>
#include <stdlib.h>

extern s_program p;

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

    if (s->rela_table->entry_num == s->rela_table->size)
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

static const char* rela_type_name(e_Elf64_reloc_type type)
{
    switch (type)
    {
    case R_HIPO_NONE: return "R_HIPO_NONE";
    case R_HIPO_32: return "R_HIPO_32";
    case R_HIPO_12: return "R_HIPO_12";
    case R_HIPO_PC12: return "R_HIPO_PC12";
    }

    return "R_HIPO_UNKNOWN";
}

void print_rela_table(s_rela_table* rela_table)
{
    if (rela_table == 0)
    {
        printf("  Rela table: <none>\n");
        return;
    }

    printf("  Rela table for section %s: entries=%d, capacity=%d\n",
           rela_table->section != 0 ? rela_table->section->name : "<none>",
           rela_table->entry_num,
           rela_table->size);
    printf("    %-8s %-8s %-14s %s\n", "Offset", "Sym", "Type", "Addend");

    for (int i = 0; i < rela_table->entry_num; i++)
    {
        s_Elf64_Rela_entry* entry = rela_table->entries[i];

        if (entry == 0)
        {
            printf("    <null entry>\n");
            continue;
        }

        printf("    0x%06lx %-8d %-14s %ld\n",
               entry->r_offset,
               entry->sym_index,
               rela_type_name(entry->reloc_type),
               entry->r_addend);
    }
}

void check_rela_table(s_rela_table* rela_table)
{
    for (int i = 0; i < rela_table->entry_num; i++)
    {
        s_Elf64_Rela_entry* entry = rela_table->entries[i];
        s_Elf64_Sym* sym = p.sym_table->symbols[entry->sym_index];

        if (sym->binding = STB_GLOBAL)
            continue;

        s_section* sym_section = sym->section;
        int section_indx = check_symbol_table(sym_section->name);
        entry->sym_index = section_indx;
        entry->r_addend = sym->st_value;
    }
}
