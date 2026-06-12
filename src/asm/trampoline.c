#include "trampoline.h"
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