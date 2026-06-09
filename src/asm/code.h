#include <stdbool.h>

typedef enum {
    EXPR_LITERAL,
    EXPR_SYMBOL,
    EXPR_ADD,
    EXPR_SUB,
    EXPR_MUL,
    EXPR_DIV,
    EXPR_NEG
} expr_kind;

typedef struct expr {
    expr_kind kind;
    int literal;
    char* symbol;
    struct expr* left;
    struct expr* right;
} expr;

typedef struct 
{
    bool is_literal, is_symbol;
    long literal;
    char* symbol;
} operand_jmp;


typedef struct 
{
    bool has_dollar, has_percent, has_brackets, has_literal, has_symbol;
    long literal;
    char* simbol;
    int reg;
} operand_ls;

typedef struct 
{
    bool is_literal, is_symbol;
    char* symbol;
    int literal;
} sym_or_lit;

typedef struct 
{
    bool is_instruction, is_directive, is_label;
    char operation[10];
    int reg1, reg2, csreg;
    operand_jmp o_jmp;
    operand_ls o_ls;

    // for equ
    char* new_symbol;
    expr expression;

    // for extern, global
    char** symbol_list;
    int symbol_list_n;

    // for section
    char* section_name;

    // for ascii
    char* ascii_string;

    // for skip
    int byte_num;
    
    // for word
    sym_or_lit* sym_or_lit_list;
    int sym_or_lit_list_n;

} asm_line;