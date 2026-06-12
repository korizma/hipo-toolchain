#include "elf.h"
#include <stdio.h>
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

static const char* symbol_type_name(e_Elf64_SymbolType type)
{
    switch (type)
    {
    case STT_NOTYPE: return "NOTYPE";
    case STT_SECTION: return "SECTION";
    }

    return "UNKNOWN";
}

static const char* symbol_binding_name(e_Elf64_SymbolBinding binding)
{
    switch (binding)
    {
    case STB_LOCAL: return "LOCAL";
    case STB_GLOBAL: return "GLOBAL";
    case STB_WEAK: return "WEAK";
    case STB_LOOS: return "LOOS";
    case STB_HIOS: return "HIOS";
    case STB_LOPROC: return "LOPROC";
    case STB_HIPROC: return "HIPROC";
    }

    return "UNKNOWN";
}

static const char* symbol_state_name(e_Elf64_symbol_entry_state state)
{
    switch (state)
    {
    case ST_ENTRY_STATE_EMPTY: return "EMPTY";
    case ST_ENTRY_STATE_PARTIAL_GLOBAL: return "PARTIAL_GLOBAL";
    case ST_ENTRY_STATE_PARTIAL_REFERENCE: return "PARTIAL_REFERENCE";
    case ST_ENTRY_STATE_COMPLETE: return "COMPLETE";
    }

    return "UNKNOWN";
}

void print_program()
{
    printf("Program: sections=%d, section_capacity=%d, symbols=%d\n",
           p.number_of_sections,
           p.section_arr_size,
           p.sym_table != 0 ? p.sym_table->symbol_num : 0);

    printf("\nSections:\n");
    for (int i = 0; i < p.number_of_sections; i++)
    {
        print_section(p.sections[i]);
    }

    printf("\nSymbol table:\n");
    if (p.sym_table == 0)
    {
        printf("  <none>\n");
        return;
    }

    printf("  %-5s %-20s %-8s %-8s %-16s %-8s %-8s %s\n",
           "Index", "Name", "Type", "Bind", "Section", "Value", "Size", "State");

    for (int i = 0; i < p.sym_table->symbol_num; i++)
    {
        s_Elf64_Sym* symbol = p.sym_table->symbols[i];

        if (symbol == 0)
        {
            printf("  %-5d <null symbol>\n", i);
            continue;
        }

        printf("  %-5d %-20s %-8s %-8s %-16s %-8ld %-8ld %s\n",
               i,
               symbol->st_name != 0 ? symbol->st_name : "<none>",
               symbol_type_name(symbol->type),
               symbol_binding_name(symbol->binding),
               symbol->section != 0 ? symbol->section->name : "<none>",
               symbol->st_value,
               symbol->st_size,
               symbol_state_name(symbol->state));
    }
}
