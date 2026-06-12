#include "handlers_impl.h"
#include "code.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern s_program p;

int handle_extern(s_asm_line* line, s_section* s)
{
    int n = line->symbol_list_n;
    for (int i = 0; i < n; i++)
    {
        if (check_symbol_table(line->symbol_list[i]) != -1)
        {
            printf("ERROR: symbol %s already defined, cannot be declared as an extern symbol!\n", line->symbol_list[i]);
            return -1;
        }
        add_to_symbol_table(line->symbol_list[i], 0, STB_GLOBAL, 0, 0, 0, 0, ST_ENTRY_STATE_COMPLETE);
    }
    return 0;
}

int handle_global(s_asm_line* line, s_section* s)
{
    int n = line->symbol_list_n;
    for (int i = 0; i < n; i++)
    {
        int indx = check_symbol_table(line->symbol_list[i]);
        if (indx != -1)
        {
            if (p.sym_table->symbols[indx]->type == STT_SECTION)
            {
                printf("ERROR: Symbol %s is a section, it cannot be declared as a global symbol!\n", line->symbol_list[i]);
                return -1;
            }
            else
            {
                p.sym_table->symbols[indx]->binding = STB_GLOBAL;
            }
        }
        else
            add_to_symbol_table(line->symbol_list[i], 0, STB_GLOBAL, 0, 0, 0, 0, ST_ENTRY_STATE_PARTIAL_GLOBAL);
    }
    return 0;
}

int handle_section(s_asm_line* line, s_section* s)
{
    if (s != 0)
        update_section_size_in_sym_table(s);

    s_section* created_section = new_section(line->section_name);
    p.curr_section = created_section;
    add_to_symbol_table(line->section_name, STT_SECTION, STB_LOCAL, STV_DEFAULT, 0, 0, 0, ST_ENTRY_STATE_COMPLETE);
}

int handle_word(s_asm_line* line, s_section* s)
{
    if (s == 0)
    {
        printf("ERROR: Directive .word used outside of a section!\n");
        return -1;
    }

    update_label_size_if_last(s, line->sym_or_lit_list_n * WORD_SIZE);

    int n = line->sym_or_lit_list_n;
    for (int i = 0; i < n; i++)
    {
        if (line->sym_or_lit_list[i]->is_literal)
        {
            long literal = line->sym_or_lit_list[i]->literal;
            char bytes[4];
            bytes[0] = literal & 0xFF;
            bytes[1] = (literal >> 8) & 0xFF;
            bytes[2] = (literal >> 16) & 0xFF;
            bytes[3] = (literal >> 24) & 0xFF;
            write_bytes_to_section(s, bytes, 4);
        }
        else if (line->sym_or_lit_list[i]->is_symbol)
        {
            int indx = get_and_set_reference(line->sym_or_lit_list[i]->symbol);

            s_Elf64_Sym* sym = p.sym_table->symbols[indx];
            // create relocation for this symbol
            // for now just skip
            skip_bytes_in_section(s, WORD_SIZE);
        }
    }
    return 0;
}

int handle_skip(s_asm_line* line, s_section* s)
{
    if (s == 0)
    {
        printf("ERROR: Directive .skip used outside of a section!\n");
        return -1;
    }

    update_label_size_if_last(s, line->byte_num);

    skip_bytes_in_section(s, line->byte_num);
    return 0;
}

int handle_ascii(s_asm_line* line, s_section* s)
{
    if (s == 0)
    {
        printf("ERROR: Directive .ascii used outside of a section!\n");
        return -1;
    }

    update_label_size_if_last(s, strlen(line->ascii_string) + 1);

    write_bytes_to_section(s, line->ascii_string, strlen(line->ascii_string));
    return 0;
}

int handle_equ(s_asm_line* line, s_section* s)
{
    return 0;
}

int handle_end(s_asm_line* line, s_section* s)
{
    if (s == 0)
    {
        printf("ERROR: .end has no section to end!\n");
        return -1;
    }
    p.curr_section = 0;
    return 0;
}

int handle_label(s_asm_line* line, s_section* s)
{
    if (s == 0)
    {
        printf("ERROR: Label %s defined outside of a section!\n", line->symbol);
        return -1;
    }

    int indx = check_symbol_table(line->symbol);
    if (indx == -1)
    {
        add_to_symbol_table(line->symbol, STT_NOTYPE, STB_LOCAL, STV_DEFAULT, s, s->next_free, 0, ST_ENTRY_STATE_COMPLETE);
    }
    else
    {
        s_Elf64_Sym* sym = p.sym_table->symbols[indx];
        if (sym->state == ST_ENTRY_STATE_COMPLETE)
        {
            printf("ERROR: symbol %s already defined defined!\n", line->symbol);
            return -1;
        }

        if (sym->state != ST_ENTRY_STATE_PARTIAL_GLOBAL)
            sym->binding = STB_LOCAL;

        sym->type = STT_NOTYPE;
        sym->visibility = STV_DEFAULT;
        sym->section = s;
        sym->st_value = s->next_free;
        sym->st_size = 0;
    }

    // sym size needs to be set only if the next directive is ascii, word or skip
    return 0;
}
