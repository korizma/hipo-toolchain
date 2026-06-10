#ifndef HELPER_H
#define HELPER_H

#include "../asm/code.h"
#include "stdlib.h"
#include "string.h"

typedef struct {
    char* reg1;
    char* reg2;
} gpr_pair;

typedef struct {
    sym_or_lit** arr;
    int n, curr_size;
} list_n_sl;

typedef struct {
    char** arr;
    int n, curr_size;
} list_n_s;

expr* expr_literal(int value);
expr* expr_symbol(char* symbol);
expr* expr_binary(expr_kind kind, expr* left, expr* right);

asm_line* new_empty_line(void);
void add_line(asm_line* line);

int reg_to_int(char* reg);

void allocate_n_more_sym_list(list_n_s* list, int n_more);
void allocate_n_more_sl_list(list_n_sl* list, int n_more);

void add_to_list_sym_list(list_n_s* list, char* sym);
void add_to_list_sl_list(list_n_sl* list, sym_or_lit* a);

sym_or_lit* new_lit_sl(int lit);
sym_or_lit* new_sym_sl(char* sym);

void reverse_arr_sl(list_n_sl* list);
void reverse_arr_sym(list_n_s* list);

int parse_literal_lex(char* lit);

void yyerror(const char *message);

#endif
