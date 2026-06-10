#ifndef CODE_H
#define CODE_H

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

typedef enum {
    ASM_INSTR_NONE = 0,
    ASM_INSTR_HALT,
    ASM_INSTR_INT,
    ASM_INSTR_IRET,
    ASM_INSTR_CALL,
    ASM_INSTR_RET,
    ASM_INSTR_JMP,
    ASM_INSTR_BEQ,
    ASM_INSTR_BNE,
    ASM_INSTR_BGT,
    ASM_INSTR_PUSH,
    ASM_INSTR_POP,
    ASM_INSTR_XCHG,
    ASM_INSTR_ADD,
    ASM_INSTR_SUB,
    ASM_INSTR_MUL,
    ASM_INSTR_DIV,
    ASM_INSTR_NOT,
    ASM_INSTR_AND,
    ASM_INSTR_OR,
    ASM_INSTR_XOR,
    ASM_INSTR_SHL,
    ASM_INSTR_SHR,
    ASM_INSTR_LD,
    ASM_INSTR_ST,
    ASM_INSTR_CSRRD,
    ASM_INSTR_CSRWR
} asm_instruction;

typedef enum {
    ASM_DIR_NONE = 0,
    ASM_DIR_GLOBAL,
    ASM_DIR_EXTERN,
    ASM_DIR_SECTION,
    ASM_DIR_WORD,
    ASM_DIR_SKIP,
    ASM_DIR_ASCII,
    ASM_DIR_EQU,
    ASM_DIR_END
} asm_directive;

typedef enum {
    ASM_REG_R0 = 0,
    ASM_REG_R1,
    ASM_REG_R2,
    ASM_REG_R3,
    ASM_REG_R4,
    ASM_REG_R5,
    ASM_REG_R6,
    ASM_REG_R7,
    ASM_REG_R8,
    ASM_REG_R9,
    ASM_REG_R10,
    ASM_REG_R11,
    ASM_REG_R12,
    ASM_REG_R13,
    ASM_REG_R14,
    ASM_REG_SP = ASM_REG_R14,
    ASM_REG_R15,
    ASM_REG_PC = ASM_REG_R15,
    ASM_REG_STATUS,
    ASM_REG_HANDLER,
    ASM_REG_CAUSE
} asm_register;

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
    char* symbol;
    asm_register reg;
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
    char* symbol;
    asm_instruction instruction;
    asm_directive directive;
    asm_register reg1, reg2, csreg;
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
    sym_or_lit** sym_or_lit_list;
    int sym_or_lit_list_n;

} asm_line;

const char* asm_instruction_name(asm_instruction instruction);
const char* asm_directive_name(asm_directive directive);
void print_asm_line(asm_line* line);


#endif
