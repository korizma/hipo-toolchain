#ifndef HELPER_H
#define HELPER_H

#include "../asm/code.h"
#include "stdlib.h"
#include "string.h"

typedef struct {
    asm_register reg1;
    asm_register reg2;
} s_gpr_pair;

typedef struct {
    s_sym_or_lit** arr;
    int n, curr_size;
} s_list_n_sl;

typedef struct {
    char** arr;
    int n, curr_size;
} s_list_n_s;

s_expr* expr_literal(int value);
s_expr* expr_symbol(char* symbol);
s_expr* expr_binary(expr_kind kind, s_expr* left, s_expr* right);

s_asm_line* new_empty_line(void);
void add_line(s_asm_line* line);

asm_register reg_from_name(char* reg);

void allocate_n_more_sym_list(s_list_n_s* list, int n_more);
void allocate_n_more_sl_list(s_list_n_sl* list, int n_more);

void add_to_list_sym_list(s_list_n_s* list, char* sym);
void add_to_list_sl_list(s_list_n_sl* list, s_sym_or_lit* a);

s_sym_or_lit* new_lit_sl(int lit);
s_sym_or_lit* new_sym_sl(char* sym);

void reverse_arr_sl(s_list_n_sl* list);
void reverse_arr_sym(s_list_n_s* list);

int parse_literal_lex(char* lit);

void yyerror(const char *message);

#endif
