#ifndef CODE_H
#define CODE_H

#include <stdbool.h>
#include "section.h"

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
} e_asm_instruction;

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
} e_asm_directive;

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
} e_asm_register;

typedef enum  
{
    EXPR_VALID_KNOWN,
    EXPR_VALID_UNKNOWN,
    EXPR_INVALID,
    EXPR_UNKNOWN
} e_expr_status;


// expr handling
typedef struct 
{
    char** symbol_list; // list of symbol names used in the expression
    int* symbol_coeff;  // 1*x, -1*x, 2*x...
    int symbol_num;     // number of symbols
    int lists_size;
    long value;         // value to be added to the symbols
} s_expr;

typedef struct 
{
    bool is_literal, is_symbol;
    long literal;
    char* symbol;
} s_operand_jmp;

typedef enum {
    ASM_OPERAND_LS_NONE = 0,
    ASM_OPERAND_LS_IMM_LITERAL,
    ASM_OPERAND_LS_IMM_SYMBOL,
    ASM_OPERAND_LS_MEM_LITERAL,
    ASM_OPERAND_LS_MEM_SYMBOL,
    ASM_OPERAND_LS_REG,
    ASM_OPERAND_LS_REG_INDIRECT,
    ASM_OPERAND_LS_REG_INDIRECT_LITERAL,
    ASM_OPERAND_LS_REG_INDIRECT_SYMBOL
} e_operand_ls_kind;

typedef struct 
{
    e_operand_ls_kind kind;
    long literal;
    char* symbol;
    e_asm_register reg;
} s_operand_ls;

typedef struct 
{
    bool is_literal, is_symbol;
    char* symbol;
    long literal;
} s_sym_or_lit;

typedef struct 
{
    bool is_instruction, is_directive, is_label;
    char* symbol;
    e_asm_instruction instruction;
    e_asm_directive directive;
    e_asm_register reg1, reg2, csreg;
    s_operand_jmp o_jmp;
    s_operand_ls o_ls;

    // for equ
    char* new_symbol;
    s_expr expression;

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
    s_sym_or_lit** sym_or_lit_list;
    int sym_or_lit_list_n;

    // for error handling and debugging
    int line_num;

    // for later relocation cleanup
    s_section* section_location;
    int bytes_location;

} s_asm_line;

const char* asm_instruction_name(e_asm_instruction instruction);
const char* asm_directive_name(e_asm_directive directive);

// Caller owns the returned string and must free it.
char* asm_line_to_string(s_asm_line* line);


#endif
