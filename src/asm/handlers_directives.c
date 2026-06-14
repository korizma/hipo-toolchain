#include "handlers_impl.h"
#include "code.h"
#include <stdlib.h>
#include <string.h>

extern s_program p;

s_error* handle_extern(s_asm_line* line, s_section* s)
{
    int n = line->symbol_list_n;
    for (int i = 0; i < n; i++)
    {
        if (check_symbol_table(line->symbol_list[i]) != -1)
        {
            return new_symbol_error(line, ERR_INVALID_EXTERN_SYMBOL, line->symbol_list[i]);
        }
        add_to_symbol_table(line->symbol_list[i], 0, STB_GLOBAL, 0, 0, 0, 0, ST_ENTRY_STATE_COMPLETE);
    }
    return NULL;
}

s_error* handle_global(s_asm_line* line, s_section* s)
{
    int n = line->symbol_list_n;
    for (int i = 0; i < n; i++)
    {
        int indx = check_symbol_table(line->symbol_list[i]);
        if (indx != -1)
        {
            if (p.sym_table->symbols[indx]->type == STT_SECTION)
            {
                return new_symbol_error(line, ERR_INVALID_GLOBAL_SYMBOL, line->symbol_list[i]);
            }
            else
            {
                p.sym_table->symbols[indx]->binding = STB_GLOBAL;
            }
        }
        else
            add_to_symbol_table(line->symbol_list[i], 0, STB_GLOBAL, 0, 0, 0, 0, ST_ENTRY_STATE_PARTIAL_GLOBAL);
    }
    return NULL;
}

s_error* handle_section(s_asm_line* line, s_section* s)
{
    if (s != 0)
        update_section_size_in_sym_table(s);

    int indx = check_symbol_table(line->section_name);

    if (indx != -1)
    {
        return new_symbol_error(line, ERR_DUPLICATE_SECTION, line->section_name);
    }

    s_section* created_section = new_section(line->section_name);
    add_section_to_program(created_section);
    p.curr_section = created_section;
    add_to_symbol_table(line->section_name, STT_SECTION, STB_LOCAL, STV_DEFAULT, 0, 0, 0, ST_ENTRY_STATE_COMPLETE);

    return NULL;
}

s_error* handle_word(s_asm_line* line, s_section* s)
{
    if (s == 0)
    {
        return new_error(line, ERR_OUTSIDE_SECTION);
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

            if (sym->type != STT_SECTION)
                sym->st_size = WORD_SIZE;
            
            // suppose the symbol is global, if not change in cleanup
            create_rela_entry(s, s->next_free, indx, R_HIPO_32, 0);

            char bin[WORD_SIZE] = {0, 0, 0, 0};
            write_bytes_to_section(s, bin, WORD_SIZE);
        }
    }
    return NULL;
}

s_error* handle_skip(s_asm_line* line, s_section* s)
{
    if (s == 0)
    {
        return new_error(line, ERR_OUTSIDE_SECTION);
    }

    if (line->byte_num <= 0 || line->byte_num >= SKIP_BYTE_LIMIT)
    {
        return new_error(line, ERR_SKIP_BYTE_LIMIT);   
    }

    update_label_size_if_last(s, line->byte_num);

    skip_bytes_in_section(s, line->byte_num);
    return NULL;
}

s_error* handle_ascii(s_asm_line* line, s_section* s)
{
    if (s == 0)
    {
        return new_error(line, ERR_OUTSIDE_SECTION);
    }

    update_label_size_if_last(s, strlen(line->ascii_string));

    write_bytes_to_section(s, line->ascii_string, strlen(line->ascii_string));
    return NULL;
}

s_error* handle_equ(s_asm_line* line, s_section* s)
{
    bool invalid = expr_is_invalid(line->expression);
    if (invalid)
    {
        return new_symbol_error(line, ERR_EXPR_INVALID, line->new_symbol);
    }

    int indx = check_symbol_table(line->new_symbol);
    if (indx == -1)
    {
        add_to_symbol_table(line->new_symbol, STT_NOTYPE, STB_LOCAL, STV_DEFAULT, 0, 0, 0, ST_ENTRY_STATE_EQU);
        int indx = check_symbol_table(line->new_symbol);
        p.sym_table->symbols[indx]->equ_expr = line->expression;
    }
    else
    {
        s_Elf64_Sym* sym = p.sym_table->symbols[indx];
        if (sym->state == ST_ENTRY_STATE_COMPLETE)
        {
            return new_symbol_error(line, ERR_DUPLICATE_SYMBOL, line->symbol);
        }

        if (sym->binding != STB_GLOBAL)
            sym->binding = STB_LOCAL;

        sym->type = STT_NOTYPE;
        sym->visibility = STV_DEFAULT;
        sym->state = ST_ENTRY_STATE_EQU;
        sym->equ_expr = line->expression;
    }

    return NULL;
}

s_error* handle_end(s_asm_line* line, s_section* s)
{
    if (s == 0)
    {
        return new_error(line, ERR_OUTSIDE_SECTION);
    }
    update_section_size_in_sym_table(s);

    p.curr_section = 0;
    p.has_ended = true;
    return NULL;
}

s_error* handle_label(s_asm_line* line, s_section* s)
{
    if (s == 0)
    {
        return new_symbol_error(line, ERR_OUTSIDE_SECTION, line->symbol);
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
            return new_symbol_error(line, ERR_DUPLICATE_SYMBOL, line->symbol);
        }

        if (sym->binding != STB_GLOBAL)
            sym->binding = STB_LOCAL;

        sym->type = STT_NOTYPE;
        sym->visibility = STV_DEFAULT;
        sym->section = s;
        sym->st_value = s->next_free;
        sym->st_size = 0;

        sym->state = ST_ENTRY_STATE_COMPLETE;
    }

    // sym size needs to be set only if the next directive is ascii, word or skip
    return NULL;
}
