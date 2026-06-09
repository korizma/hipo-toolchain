#include "code.h"
#include "stdlib.h"

extern asm_line* lines[];
extern int curr_line;

typedef struct {
    char* reg1;
    char* reg2;
} gpr_pair;

expr* expr_literal(int value) {
    expr* e = malloc(sizeof(expr));
    e->kind = EXPR_LITERAL;
    e->literal = value;
    e->symbol = 0;
    e->left = 0;
    e->right = 0;
    return e;
}

expr* expr_symbol(char* symbol) {
    expr* e = malloc(sizeof(expr));
    e->kind = EXPR_SYMBOL;
    e->literal = 0;
    e->symbol = symbol;
    e->left = 0;
    e->right = 0;
    return e;
}

expr* expr_binary(expr_kind kind, expr* left, expr* right) {
    expr* e = malloc(sizeof(expr));
    e->kind = kind;
    e->literal = 0;
    e->symbol = 0;
    e->left = left;
    e->right = right;
    return e;
}

asm_line* new_empty_line()
{
    return (asm_line*)calloc(1, sizeof(asm_line));
}

void add_line(asm_line* line)
{
    lines[curr_line++] = line;
}