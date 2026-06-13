#include "trampoline.h"
#include "rela_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "handlers_impl.h"

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

int write_trampolines()
{
    s_error* error = NULL;
    bool has_error = false;
    for (int i = 0; i < trampoline.entry_num; i++)
    {
        s_trampoline_entry* entry = trampoline.entries[i];

        if (entry == 0)
            continue;

        switch (entry->type)
        {
        case TE_JUMP_LITERAL:
            error = write_trampoline_literal(entry);
            if (error != NULL)
                has_error = true;
            break;
        case TE_JUMP_SYMBOL:
            error = write_trampoline_symbol(entry);
            if (error != NULL)
                has_error = true;
            break;
        case TE_LD_IMM_LITERAL:
            error = write_trampoline_literal(entry);
            if (error != NULL)
                has_error = true;
            break;
        case TE_LD_IMM_SYMBOL:
            error = write_trampoline_symbol(entry);
            if (error != NULL)
                has_error = true;   
            break;
        case TE_ST_MEM_LITERAL:
            error = write_trampoline_literal(entry);
            if (error != NULL)
                has_error = true;
            break;
        case TE_ST_MEM_SYMBOL:
            error = write_trampoline_symbol(entry);
            if (error != NULL)
                has_error = true;
            break;
        }

        if (error != NULL)
           free(error);
    }

    
    if (!has_error)
        return 0;

    return -1;
}

static const char* trampoline_type_name(e_trampoline_entry_type type)
{
    switch (type)
    {
    case TE_JUMP_SYMBOL: return "JUMP_SYMBOL";
    case TE_JUMP_LITERAL: return "JUMP_LITERAL";
    case TE_LD_IMM_SYMBOL: return "LD_IMM_SYMBOL";
    case TE_LD_IMM_LITERAL: return "LD_IMM_LITERAL";
    case TE_ST_MEM_SYMBOL: return "ST_MEM_SYMBOL";
    case TE_ST_MEM_LITERAL: return "ST_MEM_LITERAL";
    }

    return "UNKNOWN";
}

static bool trampoline_entry_uses_symbol(e_trampoline_entry_type type)
{
    return type == TE_JUMP_SYMBOL ||
           type == TE_LD_IMM_SYMBOL ||
           type == TE_ST_MEM_SYMBOL;
}

static void print_optional_hex_long(long value)
{
    if (value < 0)
        printf("%-10s", "-");
    else
        printf("0x%08lx", (unsigned long)value);
}

static void print_optional_hex_int(bool has_value, int value)
{
    if (!has_value)
        printf("%-10s", "-");
    else
        printf("0x%08x", value);
}

static void print_trampoline_source_line(s_asm_line* line)
{
    if (line == 0)
    {
        printf("      source: <none>\n");
        return;
    }

    char* source = asm_line_to_string(line);
    if (source == NULL)
    {
        printf("      source: <unavailable>\n");
        return;
    }

    printf("      source: %s", source);

    if (source[0] == '\0' || source[strlen(source) - 1] != '\n')
        printf("\n");

    free(source);
}

void print_trampoline()
{
    printf("\nTrampoline table\n");
    printf("  entries=%d, capacity=%d\n", trampoline.entry_num, trampoline.size);

    if (trampoline.entry_num == 0)
    {
        printf("  <empty>\n");
        return;
    }

    printf("  %-3s %-14s %-14s %-10s %-10s %-8s %-8s %s\n",
           "#",
           "Type",
           "Section",
           "Use@",
           "Pool@",
           "Disp",
           "State",
           "Target");
    printf("  %-3s %-14s %-14s %-10s %-10s %-8s %-8s %s\n",
           "---",
           "--------------",
           "--------------",
           "----------",
           "----------",
           "--------",
           "--------",
           "----------------");

    for (int i = 0; i < trampoline.entry_num; i++)
    {
        s_trampoline_entry* entry = trampoline.entries[i];

        if (entry == 0)
        {
            printf("  %-3d <null entry>\n", i);
            continue;
        }

        printf("  %-3d %-14s %-14s ",
               i,
               trampoline_type_name(entry->type),
               entry->section != 0 ? entry->section->name : "<none>");

        print_optional_hex_int(entry->line != 0, entry->line != 0 ? entry->line->bytes_location : 0);
        printf(" ");

        print_optional_hex_long(entry->trampoline_location);
        printf(" ");

        if (entry->line != 0 && entry->trampoline_location >= 0)
            printf("%-8ld", entry->trampoline_location - entry->line->bytes_location);
        else
            printf("%-8s", "-");

        printf(" %-8s ",
               entry->is_done ? "done" : "pending");

        if (trampoline_entry_uses_symbol(entry->type))
        {
            printf("symbol %s\n", entry->symbol != 0 ? entry->symbol : "<none>");
        }
        else
        {
            printf("literal %ld (0x%08lx)\n",
                   entry->literal,
                   (unsigned long)entry->literal);
        }

        print_trampoline_source_line(entry->line);
    }
}

s_error* write_trampoline_literal(s_trampoline_entry* entry)
{
    s_trampoline_entry* match = find_matching_trampoline_entry(entry);
    if (match != NULL)
    {
        write_displacement_to_line(entry->line, match->trampoline_location);
        entry->trampoline_location = match->trampoline_location;
        entry->is_done = true;
        return NULL;
    }

    long literal = entry->literal;
    char bin[4];
    bin[0] = literal & 0xFF;
    bin[1] = (literal >> 8) & 0xFF;
    bin[2] = (literal >> 16) & 0xFF;
    bin[3] = (literal >> 24) & 0xFF;

    entry->trampoline_location = entry->section->next_free;
    write_bytes_to_section(entry->section, bin, TRAMPOLINE_ONE_ENTRY_MEM_SIZE);
    
    update_section_size_in_sym_table(entry->section);

    write_displacement_to_line(entry->line, entry->trampoline_location);
    entry->is_done = true;
    return NULL;
}

s_error* write_trampoline_symbol(s_trampoline_entry* entry)
{
    s_trampoline_entry* match = find_matching_trampoline_entry(entry);
    if (match != NULL)
    {
        s_error* err = write_displacement_to_line(entry->line, match->trampoline_location);
        if (err != NULL)
            return err;
        entry->trampoline_location = match->trampoline_location;
        entry->is_done = true;
        return NULL;
    }

    char bin[4] = {0,0,0,0};

    entry->trampoline_location = entry->section->next_free;
    s_error* err = write_displacement_to_line(entry->line, entry->trampoline_location);
    if (err != NULL)
        return err;

    write_bytes_to_section(entry->section, bin, TRAMPOLINE_ONE_ENTRY_MEM_SIZE);
    
    update_section_size_in_sym_table(entry->section);


    create_rela_entry(entry->section, entry->trampoline_location, check_symbol_table(entry->symbol), R_HIPO_32, 0);
    entry->is_done = true;
    return NULL;
}


s_trampoline_entry* find_matching_trampoline_entry(s_trampoline_entry* entry)
{
    for (int i = 0; i < trampoline.entry_num; i++)
    {
        s_trampoline_entry* match = trampoline.entries[i];
        if (match == 0 || !match->is_done)
            return NULL;

        if (match->section != entry->section)
            continue;

        if (entry->symbol != 0 && match->symbol != 0 && strcmp(match->symbol, entry->symbol) == 0)
            return match;

        if (entry->symbol == 0 && match->symbol == 0 && match->literal == entry->literal)
            return match;
    }
    return NULL;
}

s_error* write_displacement_to_line(s_asm_line* line, long trampoline_location)
{
    long value_12b = trampoline_location - line->bytes_location;

    if (!long_fit_in_12b(value_12b))
    {
        return new_error(line, ERR_TRAMPOLINE_DISPLACEMENT_TOO_LARGE);
    }

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
    return NULL;
}

void free_trampoline()
{
    for (int i = 0; i < trampoline.entry_num; i++)
    {
        free(trampoline.entries[i]);
    }
    free(trampoline.entries);
}
