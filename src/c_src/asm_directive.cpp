#include "asm_directive.hpp"
#include "error.hpp"
#include "symbol_table.hpp"
#include "asm.hpp"
#include "section.hpp"
#include "misc.hpp"
#include "rela_table.hpp"

s_asm_directive* new_asm_directive()
{
    s_asm_directive* new_dir = (s_asm_directive*)malloc(sizeof(s_asm_directive));
    return new_dir;
}

s_error handle_asm_directive(s_asm_directive* directive)
{
    s_error err;
    switch (directive->directive)
    {
    case ASM_DIR_ASCII:
        err = handle_ascii(directive);
        break;
    case ASM_DIR_EQU:
        err = handle_equ(directive);
        break;
    case ASM_DIR_EXTERN:
        err = handle_extern(directive);
        break;
    case ASM_DIR_GLOBAL:
        err = handle_global(directive);
        break;
    case ASM_DIR_SECTION:
        err = handle_section(directive);
        break;
    case ASM_DIR_SKIP:
        err = handle_skip(directive);
        break;
    case ASM_DIR_WORD:
        err = handle_word(directive);
        break;
    }

    return err;
}


s_error handle_global(s_asm_directive* directive)
{
    for (string symbol_name : directive->symbol_list)
    {
        s_symbol_table_entry* symbol = get_and_create_new_symbol_entry(symbol_name);

        symbol->binding = STB_GLOBAL;
        if (symbol->state != STS_COMPLETE)
            symbol->state = STS_GLOBAL_REFERENCE;
    }

    return new_no_error();
}

s_error handle_extern(s_asm_directive* directive)
{
    for (string symbol_name : directive->symbol_list)
    {
        s_symbol_table_entry* symbol = get_and_create_new_symbol_entry(symbol_name);

        if (symbol->state == STS_COMPLETE)
            return new_error(ERR_DOUBLE_SYM_DECL, symbol_name);

        symbol->state = STS_COMPLETE;
        symbol->binding = STB_GLOBAL;
        symbol->offset_or_value = 0;
        symbol->section_symbol_table_index = -1;
        symbol->size = 0;
        symbol->type = STT_NOTYPE;
    }

    return new_no_error();
}

s_error handle_section(s_asm_directive* directive)
{
    s_symbol_table_entry* symbol = get_symbol_entry_symbol(directive->section_symbol);

    if (symbol != NULL)
        return new_error(ERR_DOUBLE_SYM_DECL, directive->section_symbol);

    symbol = create_new_symbol_entry(directive->section_symbol);

    symbol->binding = STB_LOCAL;
    symbol->offset_or_value = 0;
    symbol->section_symbol_table_index = get_symbol_entry_index_by_symbol(directive->section_symbol);
    symbol->state = STS_COMPLETE;
    symbol->size = 0;
    symbol->type = STT_SECTION;

    s_section* section = new_section();
    section->sym_table_index = symbol->section_symbol_table_index;

    add_section_to_program(section);

    return new_no_error();
}

s_error handle_word(s_asm_directive* directive)
{
    s_section* curr_section = get_current_section();

    if (curr_section == NULL)
        return new_error(ERR_LINE_OUTSIDE_SECTION);

    for (s_sym_lit sym_lit : directive->sym_lit_list)
    {
        if (sym_lit.type == SL_SYM)
        {
            s_symbol_table_entry* symbol = get_and_create_new_symbol_entry(sym_lit.symbol);

            if (symbol->state == 0) 
                symbol->state = STS_REFERENCE;

            long sym_index = get_symbol_entry_index_by_symbol(sym_lit.symbol);

            create_new_rela_table_entry(curr_section, 0, get_section_offset(curr_section), R_HIPO_32, sym_index);
            skip_bytes_in_section(curr_section, 4);
        }
        if (sym_lit.type == SL_LIT)
        {
            vector<char> literal_bytes = int_to_4_bytes(sym_lit.literal);
            
            write_bytes_to_section(curr_section, literal_bytes);
        }
    }
}

s_error handle_skip(s_asm_directive* directive)
{
    s_section* curr_section = get_current_section();

    if (curr_section == NULL)
        return new_error(ERR_LINE_OUTSIDE_SECTION);

    skip_bytes_in_section(curr_section, directive->skip_literal);

    return new_no_error();
}

s_error handle_ascii(s_asm_directive* directive)
{
    s_section* curr_section = get_current_section();

    if (curr_section == NULL)
        return new_error(ERR_LINE_OUTSIDE_SECTION);

    vector<char> ascii_vector (directive->ascii_string.begin(), directive->ascii_string.end());
    write_bytes_to_section(curr_section, ascii_vector);

    return new_no_error();
}

s_error handle_equ(s_asm_directive* directive)
{
    s_symbol_table_entry* symbol = get_symbol_entry_symbol(directive->equ_symbol);

    if (symbol != 0 && (symbol->state == STS_COMPLETE || symbol->state == STS_EQU))
        return new_error(ERR_DOUBLE_SYM_DECL, directive->equ_symbol);

    if (symbol == 0)
    {
        symbol = create_new_symbol_entry(directive->equ_symbol);
    }

    symbol->expression = directive->expr;
    symbol->offset_or_value = 0;
    symbol->section_symbol_table_index = -1;
    symbol->state = STS_EQU;
    symbol->type = STT_NOTYPE;
    if (symbol->binding != STB_GLOBAL)
        symbol->binding = STB_LOCAL;

    return new_no_error();
}


string asm_directive_to_string(s_asm_directive* directive)
{
    return "";
}