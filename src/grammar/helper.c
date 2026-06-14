#include "../asm/code.h"
#include "stdlib.h"
#include "string.h"
#include "helper.h"
#include "stdio.h"
#include "../asm/asm_main.h"

extern s_asm_file asm_file;

s_asm_line* new_empty_line()
{
    return (s_asm_line*)calloc(1, sizeof(s_asm_line));
}

void add_line(s_asm_line* line)
{
    if (asm_file.curr_line == asm_file.size)
    {
        asm_file.size += ASM_LINES_INCREMENT;
        asm_file.lines = realloc(asm_file.lines, asm_file.size * sizeof(s_asm_line*));
    }
    line->line_num = asm_file.curr_line + 1;
    asm_file.lines[asm_file.curr_line++] = line;
}

e_asm_register reg_from_name(char* reg)
{
    if (strcmp(reg, "handler") == 0)
    {
        return ASM_REG_HANDLER;
    }
    if (strcmp(reg, "status") == 0)
    {
        return ASM_REG_STATUS;
    }
    if (strcmp(reg, "cause") == 0)
    {
        return ASM_REG_CAUSE;
    }
    if (strcmp(reg, "sp") == 0)
    {
        return ASM_REG_SP;
    }
    if (strcmp(reg, "pc") == 0)
    {
        return ASM_REG_PC;
    }
    return (e_asm_register)atoi(reg + 1);
}

void allocate_n_more_sym_list(s_list_n_s* list, int n_more)
{
    list->curr_size += n_more;
    list->arr = realloc(list->arr, list->curr_size * sizeof(char*));
}

void allocate_n_more_sl_list(s_list_n_sl* list, int n_more)
{
    list->curr_size += n_more;
    list->arr = realloc(list->arr, list->curr_size * sizeof(s_sym_or_lit*));
}

void add_to_list_sym_list(s_list_n_s* list, char* sym) 
{
    if (list->n == list->curr_size)
    {
        allocate_n_more_sym_list(list, 10);
    }
    list->arr[list->n++] = sym;
}

void add_to_list_sl_list(s_list_n_sl* list, s_sym_or_lit* a) 
{
    if (list->n == list->curr_size)
    {
        allocate_n_more_sl_list(list, 10);
    }
    list->arr[list->n++] = a;
}

s_sym_or_lit* new_lit_sl(long lit)
{
    s_sym_or_lit* ret_val = (s_sym_or_lit*)malloc(sizeof(s_sym_or_lit));
    ret_val->is_literal = true;
    ret_val->is_symbol = false;
    ret_val->literal = lit;
    return ret_val;
}

s_sym_or_lit* new_sym_sl(char* sym)
{
    s_sym_or_lit* ret_val = (s_sym_or_lit*)malloc(sizeof(s_sym_or_lit));
    ret_val->is_literal = false;
    ret_val->is_symbol = true;
    ret_val->symbol = sym;
    return ret_val;
}

void reverse_arr_sl(s_list_n_sl* list)
{
    for (int i = 0; i < list->n/2; i++)
    {
        s_sym_or_lit* temp = list->arr[i];
        list->arr[i] = list->arr[list->n - i - 1];
        list->arr[list->n - i - 1] = temp;
    }
}

void reverse_arr_sym(s_list_n_s* list)
{
    for (int i = 0; i < list->n/2; i++)
    {
        char* temp = list->arr[i];
        list->arr[i] = list->arr[list->n - i - 1];
        list->arr[list->n - i - 1] = temp;
    }
}

long parse_literal_lex(char* lit) {
    if ((strlen(lit) > 2 && lit[1] != 'x') || strlen(lit) <= 2)
        return (long)strtol(lit, NULL, 10);
    return (long)strtol(lit+2, NULL, 16);
}

void yyerror(const char *message)
{
    fprintf(stderr, "Parser error: %s\n", message);
}


s_expr* new_expr()
{
    s_expr* expr = (s_expr*)malloc(sizeof(s_expr));
    expr->symbol_list = (char**)malloc(EXPR_LIST_START_SIZE * sizeof(char*));
    expr->symbol_coeff = (int*)malloc(EXPR_LIST_START_SIZE * sizeof(int));
    expr->symbol_num = 0;
    expr->lists_size = EXPR_LIST_START_SIZE;
    expr->value = 0;
    return expr;
}
void add_to_expr_literal(s_expr* expr, long literal)
{
    expr->value += literal;
}

void add_to_expr_symbol(s_expr* expr, char* symbol, int sign)
{
    for (int i = 0; i < expr->symbol_num; i++)
    {
        if (strcmp(expr->symbol_list[i], symbol) == 0)
        {
            // sign is 1 or -1
            expr->symbol_coeff[i] += sign;
            return;
        }
    }
    
    if (expr->lists_size == expr->symbol_num)
    {
        expr->lists_size += EXPR_LIST_INCREMENT;
        expr->symbol_list = realloc(expr->symbol_list, expr->lists_size * sizeof(char*));
        expr->symbol_coeff = realloc(expr->symbol_coeff, expr->lists_size * sizeof(int));
    }

    expr->symbol_coeff[expr->symbol_num] = sign;
    expr->symbol_list[expr->symbol_num++] = strdup(symbol);
}
