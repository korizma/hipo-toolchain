#include "trampoline.h"
#include <stdio.h>
#include <stdlib.h>

s_trampoline trampoline;

void init_trampoline()
{
    trampoline.size = TRAMPOLINE_START_SIZE;
    trampoline.entry_num = 0;
    trampoline.entries = (s_trampoline_entry**)malloc(sizeof(s_trampoline_entry*) * trampoline.size);
}

void add_trampoline_entry(s_section* s, s_asm_line* line, long literal, char* symbol, e_trampoline_entry_type type)
{
    if (trampoline.entry_num == trampoline.size)
    {
        trampoline.size += TRAMPOLINE_INCREMENT;
        trampoline.entries = realloc(trampoline.entries, sizeof(s_trampoline_entry*) * trampoline.size);
    }

    s_trampoline_entry* new_entry = (s_trampoline_entry*)malloc(sizeof(s_trampoline_entry));
    new_entry->section = s;
    new_entry->line = line;
    new_entry->literal = literal;
    new_entry->symbol = symbol;
    new_entry->type = type;

    trampoline.entries[trampoline.entry_num++] = new_entry;
}

void write_trampolines()
{
    // this will be implemented later
}

static const char* trampoline_type_name(e_trampoline_entry_type type)
{
    switch (type)
    {
    case TE_JUMP_SYMBOL: return "SYMBOL";
    case TE_JUMP_LITERAL: return "LITERAL";
    case TE_LD_IMM_SYMBOL: return "LD_IMM_SYMBOL";
    case TE_LD_IMM_LITERAL: return "LD_IMM_LITERAL";
    case TE_ST_MEM_SYMBOL: return "ST_MEM_SYMBOL";
    case TE_ST_MEM_LITERAL: return "ST_MEM_LITERAL";
    }

    return "UNKNOWN";
}

void print_trampoline()
{
    printf("Trampoline: entries=%d, capacity=%d\n", trampoline.entry_num, trampoline.size);
    printf("  %-8s %-16s %-12s %s\n", "Type", "Section", "Value", "Line");

    for (int i = 0; i < trampoline.entry_num; i++)
    {
        s_trampoline_entry* entry = trampoline.entries[i];

        if (entry == 0)
        {
            printf("  <null entry>\n");
            continue;
        }

        printf("  %-8s %-16s ",
               trampoline_type_name(entry->type),
               entry->section != 0 ? entry->section->name : "<none>");

        if (entry->type == TE_JUMP_SYMBOL || entry->type == TE_LD_IMM_SYMBOL || entry->type == TE_ST_MEM_SYMBOL)
        {
            printf("%-12s ", entry->symbol != 0 ? entry->symbol : "<none>");
        }
        else
        {
            printf("%-12ld ", entry->literal);
        }

        if (entry->line != 0)
        {
            char* line = asm_line_to_string(entry->line);
            if (line != NULL)
            {
                printf("%s", line);
                free(line);
            }
        }
        else
        {
            printf("<none>\n");
        }
    }
}
