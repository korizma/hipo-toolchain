#include "trampoline.h"
#include "rela_table.h"
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
    new_entry->is_done = false;
    new_entry->trampoline_location = -1;

    trampoline.entries[trampoline.entry_num++] = new_entry;
}

void write_trampolines()
{
    for (int i = 0; i < trampoline.entry_num; i++)
    {
        s_trampoline_entry* entry = trampoline.entries[i];

        if (entry == 0)
            continue;

        switch (entry->type)
        {
        case TE_JUMP_LITERAL:
            write_trampoline_literal(entry);
            break;
        case TE_JUMP_SYMBOL:
            write_trampoline_symbol(entry);
            break;
        case TE_LD_IMM_LITERAL:
            write_trampoline_literal(entry);
            break;
        case TE_LD_IMM_SYMBOL:
            write_trampoline_symbol(entry);
            break;
        case TE_ST_MEM_LITERAL:
            write_trampoline_literal(entry);
            break;
        case TE_ST_MEM_SYMBOL:
            write_trampoline_symbol(entry);
            break;
        }
    }

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

void write_trampoline_literal(s_trampoline_entry* entry)
{
    s_trampoline_entry* match = find_matching_trampoline_entry(entry);
    if (match != NULL)
    {
        write_displacement_to_line(entry->line, match->trampoline_location);
        entry->trampoline_location = match->trampoline_location;
        entry->is_done = true;
        return;
    }

    long literal = entry->literal;
    char bin[4];
    bin[0] = literal & 0xF;
    bin[1] = (literal >> 8) & 0xFF;
    bin[2] = (literal >> 16) & 0xFF;
    bin[3] = (literal >> 24) & 0xFF;

    entry->trampoline_location = entry->section->next_free;
    write_bytes_to_section(entry->section, bin, TRAMPOLINE_ONE_ENTRY_MEM_SIZE);
    
    update_section_size_in_sym_table(entry->section);

    write_displacement_to_line(entry->line, entry->trampoline_location);
}

void write_trampoline_symbol(s_trampoline_entry* entry)
{
    s_trampoline_entry* match = find_matching_trampoline_entry(entry);
    if (match != NULL)
    {
        write_displacement_to_line(entry->line, match->trampoline_location);
        entry->trampoline_location = match->trampoline_location;
        entry->is_done = true;
        return;
    }

    char bin[4] = {0,0,0,0};

    entry->trampoline_location = entry->section->next_free;
    write_bytes_to_section(entry->section, bin, TRAMPOLINE_ONE_ENTRY_MEM_SIZE);
    
    update_section_size_in_sym_table(entry->section);

    write_displacement_to_line(entry->line, entry->trampoline_location);

    create_rela_entry(entry->section, entry->trampoline_location, check_symbol_table(entry->symbol), R_HIPO_32, 0);
}


s_trampoline_entry* find_matching_trampoline_entry(s_trampoline_entry* entry)
{
    for (int i = 0; i < trampoline.entry_num; i++)
    {
        s_trampoline_entry* match = trampoline.entries[i];
        if (!match->is_done)
            return NULL;

        if (match->section == entry->section && ((match->symbol == entry->symbol && entry->symbol != 0) || (match->literal == entry->literal && entry->literal != 0)) )
            return match;
    }
    return NULL;
}

void write_displacement_to_line(s_asm_line* line, long trampoline_location)
{
    long value_12b = trampoline_location - line->bytes_location;

    int line_location_in_section = line->bytes_location;
    s_section* line_section = line->section_location;
    // +0       +1      +2      +3
    // OC MOD   rA rB   rC c3   c2 c1
    char c1c2, c3;
    c1c2 = value_12b & 0xFF;
    c3 = (value_12b >> 8) & 0xF;

    line_section->bytes[line_location_in_section + 3] = c1c2;
    line_section->bytes[line_location_in_section + 2] &= 0xF0;
    line_section->bytes[line_location_in_section + 2] |= c3;
}

void free_trampoline()
{
    for (int i = 0; i < trampoline.entry_num; i++)
    {
        free(trampoline.entries[i]);
    }
    free(trampoline.entries);
}
