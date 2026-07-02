#include "asm_directive.hpp"
#include "error.hpp"
#include "symbol_table.hpp"
#include "asm.hpp"
#include "section.hpp"
#include "misc.hpp"
#include "rela_table.hpp"
#include "expr.hpp"

s_asm_directive* new_asm_directive()
{
    return new s_asm_directive();
}

s_error handle_asm_directive(s_program* program, s_asm_directive* directive)
{
    s_error err;
    switch (directive->directive)
    {
    case ASM_DIR_ASCII:
        err = handle_ascii(program, directive);
        break;
    case ASM_DIR_EQU:
        err = handle_equ(program, directive);
        break;
    case ASM_DIR_EXTERN:
        err = handle_extern(program, directive);
        break;
    case ASM_DIR_GLOBAL:
        err = handle_global(program, directive);
        break;
    case ASM_DIR_SECTION:
        err = handle_section(program, directive);
        break;
    case ASM_DIR_SKIP:
        err = handle_skip(program, directive);
        break;
    case ASM_DIR_WORD:
        err = handle_word(program, directive);
        break;
    case ASM_DIR_END:
        program->ended = true;
        err = new_no_error();
        break;
    }

    return err;
}


s_error handle_global(s_program* program, s_asm_directive* directive)
{
    for (string symbol_name : directive->symbol_list)
    {
        s_symbol_table_entry* symbol = get_and_create_new_symbol_entry(get_symbol_table(program), symbol_name);

        symbol->binding = STB_GLOBAL;
        if (symbol->state != STS_COMPLETE)
            symbol->state = STS_GLOBAL_REFERENCE;
    }

    return new_no_error();
}

s_error handle_extern(s_program* program, s_asm_directive* directive)
{
    for (string symbol_name : directive->symbol_list)
    {
        s_symbol_table_entry* symbol = get_and_create_new_symbol_entry(get_symbol_table(program), symbol_name);

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

s_error handle_section(s_program* program, s_asm_directive* directive)
{
    s_symbol_table_entry* symbol = get_symbol_entry_symbol(get_symbol_table(program), directive->section_symbol);

    if (symbol != NULL)
        return new_error(ERR_DOUBLE_SYM_DECL, directive->section_symbol);

    symbol = create_new_symbol_entry(get_symbol_table(program), directive->section_symbol);

    symbol->binding = STB_LOCAL;
    symbol->offset_or_value = 0;
    symbol->section_symbol_table_index = get_symbol_entry_index_by_symbol(get_symbol_table(program), directive->section_symbol);
    symbol->state = STS_COMPLETE;
    symbol->size = 0;
    symbol->type = STT_SECTION;

    s_section* section = new_section();
    section->sym_table_index = symbol->section_symbol_table_index;

    add_section_to_program(program, section);

    return new_no_error();
}

s_error handle_word(s_program* program, s_asm_directive* directive)
{
    s_section* curr_section = get_current_section(program);

    if (curr_section == NULL)
        return new_error(ERR_LINE_OUTSIDE_SECTION);

    for (s_sym_lit sym_lit : directive->sym_lit_list)
    {
        if (sym_lit.type == SL_SYM)
        {
            s_symbol_table_entry* symbol = get_and_create_new_symbol_entry(get_symbol_table(program), sym_lit.symbol);

            if (symbol->state == 0) 
                symbol->state = STS_REFERENCE;

            long sym_index = get_symbol_entry_index_by_symbol(get_symbol_table(program), sym_lit.symbol);

            create_new_rela_table_entry(curr_section, 0, get_section_offset(curr_section), R_HIPO_32, sym_index);
            skip_bytes_in_section(curr_section, 4);
        }
        if (sym_lit.type == SL_LIT)
        {
            vector<char> literal_bytes = int_to_4_bytes(sym_lit.literal);
            
            write_bytes_to_section(curr_section, literal_bytes);
        }
    }

    return new_no_error();
}

s_error handle_skip(s_program* program, s_asm_directive* directive)
{
    s_section* curr_section = get_current_section(program);

    if (curr_section == NULL)
        return new_error(ERR_LINE_OUTSIDE_SECTION);

    skip_bytes_in_section(curr_section, directive->skip_literal);

    return new_no_error();
}

s_error handle_ascii(s_program* program, s_asm_directive* directive)
{
    s_section* curr_section = get_current_section(program);

    if (curr_section == NULL)
        return new_error(ERR_LINE_OUTSIDE_SECTION);

    vector<char> ascii_vector (directive->ascii_string.begin(), directive->ascii_string.end());
    write_bytes_to_section(curr_section, ascii_vector);

    return new_no_error();
}

s_error handle_equ(s_program* program, s_asm_directive* directive)
{
    s_symbol_table_entry* symbol = get_symbol_entry_symbol(get_symbol_table(program), directive->equ_symbol);

    if (symbol != 0 && (symbol->state == STS_COMPLETE || symbol->state == STS_EQU))
        return new_error(ERR_DOUBLE_SYM_DECL, directive->equ_symbol);

    if (symbol == 0)
    {
        symbol = create_new_symbol_entry(get_symbol_table(program), directive->equ_symbol);
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


static string string_list_to_string(vector<string> list)
{
    string result = "";

    for (int i = 0; i < list.size(); i++)
    {
        if (i != 0)
            result += ", ";

        result += list[i];
    }

    return result;
}

static string sym_lit_list_to_string(vector<s_sym_lit> list)
{
    string result = "";

    for (int i = 0; i < list.size(); i++)
    {
        if (i != 0)
            result += ", ";

        if (list[i].type == SL_SYM)
            result += list[i].symbol;
        else if (list[i].type == SL_LIT)
            result += to_string(list[i].literal);
    }

    return result;
}

static string expr_to_string(s_expr* expr)
{
    if (expr == NULL)
        return "";

    string result = "";

    for (int i = 0; i < expr->symbol.size(); i++)
    {
        long coeff = expr->coeff[i];

        if (coeff == 0)
            continue;

        if (result.size() != 0)
            result += coeff < 0 ? " - " : " + ";
        else if (coeff < 0)
            result += "-";

        long abs_coeff = coeff < 0 ? -coeff : coeff;

        if (abs_coeff != 1)
            result += to_string(abs_coeff) + "*";

        result += expr->symbol[i];
    }

    if (expr->integer_value != 0 || result.size() == 0)
    {
        if (result.size() != 0)
            result += expr->integer_value < 0 ? " - " : " + ";
        else if (expr->integer_value < 0)
            result += "-";

        long abs_value = expr->integer_value < 0 ? -expr->integer_value : expr->integer_value;
        result += to_string(abs_value);
    }

    return result;
}

string asm_directive_to_string(s_asm_directive* directive)
{
    if (directive == NULL)
        return "";

    switch (directive->directive)
    {
    case ASM_DIR_GLOBAL:
        return ".global " + string_list_to_string(directive->symbol_list);
    case ASM_DIR_EXTERN:
        return ".extern " + string_list_to_string(directive->symbol_list);
    case ASM_DIR_SECTION:
        return ".section " + directive->section_symbol;
    case ASM_DIR_WORD:
        return ".word " + sym_lit_list_to_string(directive->sym_lit_list);
    case ASM_DIR_SKIP:
        return ".skip " + to_string(directive->skip_literal);
    case ASM_DIR_ASCII:
        return ".ascii \"" + directive->ascii_string + "\"";
    case ASM_DIR_EQU:
        return ".equ " + directive->equ_symbol + ", " + expr_to_string(directive->expr);
    case ASM_DIR_END:
        return ".end";
    default:
        return "";
    }
}
