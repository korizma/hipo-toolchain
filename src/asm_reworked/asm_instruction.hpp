#pragma once

#include <string>
#include <vector>

using namespace std;

enum 
{
    ASM_OP_JMP_NONE = 0,
    ASM_OP_JMP_LIT,
    ASM_OP_JMP_SYM
};

enum 
{
    ASM_OP_LS_NONE = 0,
    ASM_OP_LS_IMM_LIT,
    ASM_OP_LS_IMM_SYM,
    ASM_OP_LS_MEM_LIT,
    ASM_OP_LS_MEM_SYM,
    ASM_OP_LS_REG,
    ASM_OP_LS_REG_MEM,
    ASM_OP_LS_REG_MEM_LIT,
    ASM_OP_LS_REG_MEM_SYM
};

enum 
{
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
};

enum 
{
    ASM_REG_R0 = 0,
    ASM_REG_R1 = 1,
    ASM_REG_R2 = 2,
    ASM_REG_R3 = 3,
    ASM_REG_R4 = 4,
    ASM_REG_R5 = 5,
    ASM_REG_R6 = 6,
    ASM_REG_R7 = 7,
    ASM_REG_R8 = 8,
    ASM_REG_R9 = 9,
    ASM_REG_R10 = 10,
    ASM_REG_R11 = 11,
    ASM_REG_R12 = 12,
    ASM_REG_R13 = 13,
    ASM_REG_R14 = 14,
    ASM_REG_SP = ASM_REG_R14,
    ASM_REG_R15 = 15,
    ASM_REG_PC = ASM_REG_R15,

    ASM_REG_STATUS = 0,
    ASM_REG_HANDLER = 1,
    ASM_REG_CAUSE = 2
};

typedef struct s_asm_instruction
{
    char instruction;
    char reg1, reg2, control_reg;

    // load/store_operand
    char load_store_operand_type;
    long load_store_literal;
    string load_store_symbol;
    char load_store_register;

    // jump/branch operands
    char jump_branch_operand_type;
    long jump_branch_literal;
    string jump_branch_symbol;
} s_asm_instruction;

// heap allocates a new asm_instruction
s_asm_instruction* new_asm_instruction();

// forwards the instruction to the correct handler
string handle_asm_instruction(s_asm_instruction* instruction);

// returns the instruction as a string
string asm_instruction_to_string(s_asm_instruction* instruction);

string handle_halt(s_asm_instruction* instruction);
string handle_int(s_asm_instruction* instruction);
string handle_iret(s_asm_instruction* instruction);

string handle_call(s_asm_instruction* instruction);
string handle_ret(s_asm_instruction* instruction);
string handle_push(s_asm_instruction* instruction);
string handle_pop(s_asm_instruction* instruction);


string handle_jmp(s_asm_instruction* instruction);
string handle_beq(s_asm_instruction* instruction);
string handle_bne(s_asm_instruction* instruction);
string handle_bgt(s_asm_instruction* instruction);

string handle_xchg(s_asm_instruction* instruction);

string handle_add(s_asm_instruction* instruction);
string handle_sub(s_asm_instruction* instruction);
string handle_mul(s_asm_instruction* instruction);
string handle_div(s_asm_instruction* instruction);

string handle_not(s_asm_instruction* instruction);

string handle_and(s_asm_instruction* instruction);
string handle_or(s_asm_instruction* instruction);
string handle_xor(s_asm_instruction* instruction);
string handle_shl(s_asm_instruction* instruction);
string handle_shr(s_asm_instruction* instruction);

string handle_ld(s_asm_instruction* instruction);

string handle_st(s_asm_instruction* instruction);

string handle_csrrd(s_asm_instruction* instruction);
string handle_csrwr(s_asm_instruction* instruction);