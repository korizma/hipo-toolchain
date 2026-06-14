#ifndef HELPER_H
#define HELPER_H

#include "../asm/code.h"
#include "stdlib.h"
#include "string.h"

#define EXPR_LIST_START_SIZE 5
#define EXPR_LIST_INCREMENT 2

typedef struct {
    e_asm_register reg1;
    e_asm_register reg2;
} s_gpr_pair;

typedef struct {
    s_sym_or_lit** arr;
    int n, curr_size;
} s_list_n_sl;

typedef struct {
    char** arr;
    int n, curr_size;
} s_list_n_s;

s_asm_line* new_empty_line(void);
void add_line(s_asm_line* line);

e_asm_register reg_from_name(char* reg);

void allocate_n_more_sym_list(s_list_n_s* list, int n_more);
void allocate_n_more_sl_list(s_list_n_sl* list, int n_more);

void add_to_list_sym_list(s_list_n_s* list, char* sym);
void add_to_list_sl_list(s_list_n_sl* list, s_sym_or_lit* a);

s_sym_or_lit* new_lit_sl(long lit);
s_sym_or_lit* new_sym_sl(char* sym);

void reverse_arr_sl(s_list_n_sl* list);
void reverse_arr_sym(s_list_n_s* list);

long parse_literal_lex(char* lit);

void yyerror(const char *message);

s_expr* new_expr();
void add_to_expr_literal(s_expr* expr, long literal);
void add_to_expr_symbol(s_expr* expr, char* symbol, int sign);


#endif
