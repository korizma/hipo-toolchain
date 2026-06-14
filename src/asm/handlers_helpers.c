#include "handlers_impl.h"
#include "code.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern s_program p;

char operation_to_oc[][2] = {
    {ASM_INSTR_HALT, 0},
    {ASM_INSTR_INT, 1},
    {ASM_INSTR_IRET, 9},
    {ASM_INSTR_CALL, 2},
    {ASM_INSTR_RET, 9},
    {ASM_INSTR_JMP, 3},
    {ASM_INSTR_BEQ, 3},
    {ASM_INSTR_BNE, 3},
    {ASM_INSTR_BGT, 3},
    {ASM_INSTR_PUSH, 8},
    {ASM_INSTR_POP, 9},
    {ASM_INSTR_XCHG, 4},
    {ASM_INSTR_ADD, 5},
    {ASM_INSTR_SUB, 5},
    {ASM_INSTR_MUL, 5},
    {ASM_INSTR_DIV, 5},
    {ASM_INSTR_NOT, 6},
    {ASM_INSTR_AND, 6},
    {ASM_INSTR_OR, 6},
    {ASM_INSTR_XOR, 6},
    {ASM_INSTR_SHL, 7},
    {ASM_INSTR_SHR, 7},
    {ASM_INSTR_LD, 9},
    {ASM_INSTR_ST, 8},
    {ASM_INSTR_CSRRD, 9},
    {ASM_INSTR_CSRWR, 9},
};

char operation_to_mod[][2] = {
    {ASM_INSTR_HALT, 0},
    {ASM_INSTR_INT, 0},
    {ASM_INSTR_RET, 3},
    {ASM_INSTR_PUSH, 1},
    {ASM_INSTR_POP, 3},
    {ASM_INSTR_XCHG, 0},
    {ASM_INSTR_ADD, 0},
    {ASM_INSTR_SUB, 1},
    {ASM_INSTR_MUL, 2},
    {ASM_INSTR_DIV, 3},
    {ASM_INSTR_NOT, 0},
    {ASM_INSTR_AND, 1},
    {ASM_INSTR_OR, 2},
    {ASM_INSTR_XOR, 3},
    {ASM_INSTR_SHL, 0},
    {ASM_INSTR_SHR, 1},
    {ASM_INSTR_CSRRD, 0},
    {ASM_INSTR_CSRWR, 4},
};

char* translate_to_binary(char oc, char mod, char reg_a, char reg_b, char reg_c, long disp)
{
    char* bin = (char*)malloc(sizeof(char)*INSTRUCTION_BYTE_LEN);
    // OC MOD - byte 0
    bin[0] = mod | (oc << 4);
    // regA regB - byte 1
    bin[1] = reg_b | (reg_a << 4);
    // regC DISP - byte 2
    bin[2] = ((disp >> 8) & 0b1111) | (reg_c << 4);
    // DISP DISP - byte 3
    bin[3] = disp & 0b11111111;

    return bin;
}

char find_operation_code(e_asm_instruction instr)
{
    for (int i = 0; i < sizeof(operation_to_oc) / sizeof(operation_to_oc[0]); i++) {
        if (operation_to_oc[i][0] == instr) {
            return operation_to_oc[i][1];
        }
    }
    return -1; // not found
}

char find_operation_mod(s_asm_line* line)
{
    for (int i = 0; i < sizeof(operation_to_mod) / sizeof(operation_to_mod[0]); i++) {
        if (operation_to_mod[i][0] == line->instruction) {
            return operation_to_mod[i][1];
        }
    }
    return -1; // not found
}

bool long_fit_in_12b(long a)
{
    return a >= -2048 && a <= 2047;
}


bool expr_is_invalid(s_expr* expr)
{
    s_section* found = 0;
    for (int i = 0 ; i < expr->symbol_num; i++)
    {
        int indx = get_and_set_reference(expr->symbol_list[i]);

        s_Elf64_Sym* sym = p.sym_table->symbols[indx];

        if (sym->section == 0)
            continue;

        if (found == 0)
        {
            found = sym->section;
            continue;
        }

        if (found != sym->section)
            return true;
    }
    return false;
}
void simplify_equ_expression(s_expr* expr)
{
    if (expr->symbol_num == 0)
        return;

    int indx = check_symbol_table(expr->symbol_list[0]);
    s_Elf64_Sym* sym = p.sym_table->symbols[indx];
    long refer = sym->st_value, section_sym_count = 0;

    for (int i = 1; i < expr->symbol_num; i++)
    {
        int indx_curr = check_symbol_table(expr->symbol_list[1]);
        sym = p.sym_table->symbols[indx_curr];

        expr->symbol_coeff[0] += expr->symbol_coeff[i];
        expr->value += expr->symbol_coeff[i] * (refer - sym->st_value);
    }

    if (expr->symbol_coeff[0] == 0)
        expr->symbol_num = 0;
    else
        expr->symbol_num = 1;
}

