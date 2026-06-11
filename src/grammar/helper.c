#include "../asm/code.h"
#include "stdlib.h"
#include "string.h"
#include "helper.h"
#include "stdio.h"

extern s_asm_line* lines[];
extern int curr_line;


s_expr* expr_literal(int value) {
    s_expr* e = malloc(sizeof(s_expr));
    e->kind = EXPR_LITERAL;
    e->literal = value;
    e->symbol = 0;
    e->left = 0;
    e->right = 0;
    return e;
}

s_expr* expr_symbol(char* symbol) {
    s_expr* e = malloc(sizeof(s_expr));
    e->kind = EXPR_SYMBOL;
    e->literal = 0;
    e->symbol = symbol;
    e->left = 0;
    e->right = 0;
    return e;
}

s_expr* expr_binary(expr_kind kind, s_expr* left, s_expr* right) {
    s_expr* e = malloc(sizeof(s_expr));
    e->kind = kind;
    e->literal = 0;
    e->symbol = 0;
    e->left = left;
    e->right = right;
    return e;
}

s_asm_line* new_empty_line()
{
    return (s_asm_line*)calloc(1, sizeof(s_asm_line));
}

void add_line(s_asm_line* line)
{
    lines[curr_line++] = line;
}

asm_register reg_from_name(char* reg)
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
    return (asm_register)atoi(reg + 1);
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

s_sym_or_lit* new_lit_sl(int lit)
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


int parse_literal_lex(char* lit) {
    if ((strlen(lit) > 2 && lit[1] != 'x') || strlen(lit) <= 2)
        return atoi(lit);
    return (int)strtol(lit+2, NULL, 16);
}

void yyerror(const char *message)
{
    fprintf(stderr, "Parser error: %s\n", message);
}
