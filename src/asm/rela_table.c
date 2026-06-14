#include "rela_table.h"
#include "section.h"
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
    }

    return "R_HIPO_UNKNOWN";
}

static const char* rela_symbol_name(int sym_index)
{
    if (p.sym_table == 0 ||
        sym_index < 0 ||
        sym_index >= p.sym_table->symbol_num ||
        p.sym_table->symbols[sym_index] == 0 ||
        p.sym_table->symbols[sym_index]->st_name == 0)
    {
        return "<invalid>";
    }

    return p.sym_table->symbols[sym_index]->st_name;
}

void print_rela_table(s_rela_table* rela_table)
{
    if (rela_table == 0)
    {
        printf("  Relocation table: <none>\n");
        return;
    }

    printf("  Relocation table for %s\n",
           rela_table->section != 0 ? rela_table->section->name : "<none>");
    printf("    entries=%d, capacity=%d\n",
           rela_table->entry_num,
           rela_table->size);

    if (rela_table->entry_num == 0)
    {
        printf("    <empty>\n");
        return;
    }

    printf("    %-3s %-10s %-8s %-20s %-14s %-8s\n",
           "#",
           "Offset",
           "SymIdx",
           "Symbol",
           "Type",
           "Addend");
    printf("    %-3s %-10s %-8s %-20s %-14s %-8s\n",
           "---",
           "----------",
           "--------",
           "--------------------",
           "--------------",
           "--------");

    for (int i = 0; i < rela_table->entry_num; i++)
    {
        s_Elf64_Rela_entry* entry = rela_table->entries[i];

        if (entry == 0)
        {
            printf("    %-3d <null entry>\n", i);
            continue;
        }

        printf("    %-3d 0x%08lx %-8d %-20s %-14s %-8ld\n",
               i,
               entry->r_offset,
               entry->sym_index,
               rela_symbol_name(entry->sym_index),
               rela_type_name(entry->reloc_type),
               entry->r_addend);
    }
}

void print_all_rela_tables()
{
    int printed = 0;

    printf("\nRelocation tables\n");

    if (p.number_of_sections == 0)
    {
        printf("  <no sections>\n");
        return;
    }

    for (int i = 0; i < p.number_of_sections; i++)
    {
        if (p.sections[i] == 0 || p.sections[i]->rela_table == 0)
            continue;

        print_rela_table(p.sections[i]->rela_table);
        printed = 1;
    }

    if (!printed)
        printf("  <none>\n");
}

void check_rela_table(s_rela_table* rela_table)
{
    if (rela_table == NULL)
        return;
        
    for (int i = 0; i < rela_table->entry_num; i++)
    {
        s_Elf64_Rela_entry* entry = rela_table->entries[i];

        if (entry == 0 ||
            p.sym_table == 0 ||
            entry->sym_index < 0 ||
            entry->sym_index >= p.sym_table->symbol_num)
        {
            continue;
        }

        s_Elf64_Sym* sym = p.sym_table->symbols[entry->sym_index];

        if (sym == 0 || sym->binding == STB_GLOBAL || sym->section == 0)
            continue;

        s_section* sym_section = sym->section;
        int section_indx = check_symbol_table(sym_section->name);

        if (section_indx == -1)
            continue;

        entry->sym_index = section_indx;
        entry->r_offset = sym->st_value;
    }
}


void update_const_equ_rela_entries(s_Elf64_Sym* symbol)
{
    for (int i = 0; i < p.number_of_sections; i++)
    {
        s_section* section = p.sections[i];

        if (section == 0 || section->rela_table == 0)
            continue;

        for (int j = 0; j < section->rela_table->entry_num; j++)
        {
            s_Elf64_Rela_entry* entry = section->rela_table->entries[j];
            s_Elf64_Sym* sym = p.sym_table->symbols[entry->sym_index];

            if (sym != symbol)
                continue;

            long literal = sym->equ_expr->value;

            switch (entry->reloc_type)
            {
                case R_HIPO_32:
                    section->bytes[entry->r_offset] = literal & 0xFF;
                    section->bytes[entry->r_offset + 1] = (literal >> 8) & 0xFF;
                    section->bytes[entry->r_offset + 2] = (literal >> 16) & 0xFF;
                    section->bytes[entry->r_offset + 3] = (literal >> 24) & 0xFF;
                    break;
                case R_HIPO_12:
                    section->bytes[entry->r_offset] &= 0xF0;
                    section->bytes[entry->r_offset] |= (literal & 0x0F);
                    section->bytes[entry->r_offset + 1] = (literal >> 4) & 0xFF;
                    break;
            }

            free(entry);
            section->rela_table->entries[j] = 0;
        }
    }
}

void update_all_rela_entries(s_Elf64_Sym* symbol)
{
    for (int i = 0; i < p.number_of_sections; i++)
    {
        s_section* section = p.sections[i];

        if (section == 0 || section->rela_table == 0)
            continue;

        for (int j = 0; j < section->rela_table->entry_num; j++)
        {
            s_Elf64_Rela_entry* entry = section->rela_table->entries[j];
            s_Elf64_Sym* sym = p.sym_table->symbols[entry->sym_index];

            if (sym != symbol)
                continue;

            entry->r_addend = sym->st_value;
            entry->sym_index = check_symbol_table(sym->section->name);
        }
    }
}
