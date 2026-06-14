#ifndef ERROR_H
#define ERROR_H

#include "code.h"

typedef enum 
{
    ERR_OUTSIDE_SECTION,
    ERR_DUPLICATE_SYMBOL,
    ERR_DUPLICATE_SECTION,
    ERR_INVALID_EXTERN_SYMBOL,
    ERR_INVALID_GLOBAL_SYMBOL,
    ERR_SECTION_AS_DESTINATION,
    ERR_INVALID_OPERAND,
    ERR_UNSUPPORTED_OPERAND,
    ERR_LITERAL_OUT_OF_RANGE_12B,
    ERR_MISSING_HANDLER,
    ERR_INVALID_JUMP_OPERAND,
    ERR_UNRESOLVED_SYMBOL,
    ERR_TRAMPOLINE_DISPLACEMENT_TOO_LARGE,
    ERR_SKIP_BYTE_LIMIT,
    ERR_EXPR_INVALID,
    ERR_EQU_EXTERN
} e_error_type;

typedef struct s_error
{
    e_error_type type;
    s_asm_line* line;
    const char* symbol;
    long literal;
    e_operand_ls_kind operand_kind;
} s_error;

s_error* new_error(s_asm_line* line, e_error_type type);
s_error* new_symbol_error(s_asm_line* line, e_error_type type, const char* symbol);
s_error* new_literal_error(s_asm_line* line, e_error_type type, long literal);
s_error* new_operand_error(s_asm_line* line, e_error_type type, e_operand_ls_kind operand_kind);
s_error* new_operand_literal_error(s_asm_line* line, e_error_type type, e_operand_ls_kind operand_kind, long literal);

void print_error(s_error* err);
void free_error(s_error* err);

#endif 
