#include "handlers_impl.h"
#include "code.h"

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

int handle_halt(s_asm_line* line, s_section* s)
{
    char oc = find_operation_code(line->instruction);
    char* bin = translate_to_binary(oc, 0, 0, 0, 0, 0);
    write_bytes_to_section(s, bin, INSTRUCTION_BYTE_LEN);
    return 0;
}

int handle_int(s_asm_line* line, s_section* s)
{
    char oc = find_operation_code(line->instruction);
    char* bin = translate_to_binary(oc, 0, 0, 0, 0, 0);
    write_bytes_to_section(s, bin, INSTRUCTION_BYTE_LEN);
    return 0;
}
int handle_iret(s_asm_line* line, s_section* s)
{
    // pop status
    // pop pc
}
int handle_call(s_asm_line* line, s_section* s)
{
    
}
int handle_ret(s_asm_line* line, s_section* s)
{
    // pop pc
}
int handle_jmp(s_asm_line* line, s_section* s)
{

}
int handle_beq(s_asm_line* line, s_section* s)
{

}
int handle_bne(s_asm_line* line, s_section* s)
{

}
int handle_bgt(s_asm_line* line, s_section* s)
{

}
int handle_push(s_asm_line* line, s_section* s)
{

}
int handle_pop(s_asm_line* line, s_section* s)
{

}
int handle_xchg(s_asm_line* line, s_section* s)
{

}
int handle_arthm(s_asm_line* line, s_section* s)
{
    char oc = find_operation_code(line->instruction);
    char mod = find_operation_mod(line);
    char* bin = translate_to_binary(oc, mod, line->reg1, line->reg1, line->reg2, 0);
    write_bytes_to_section(s, bin, INSTRUCTION_BYTE_LEN);
    return 0;
}
int handle_not(s_asm_line* line, s_section* s)
{
    char oc = find_operation_code(line->instruction);
    char mod = find_operation_mod(line);
    char* bin = translate_to_binary(oc, mod, line->reg1, line->reg1, 0, 0);
    write_bytes_to_section(s, bin, INSTRUCTION_BYTE_LEN);
    return 0;
}
int handle_logic(s_asm_line* line, s_section* s)
{
    char oc = find_operation_code(line->instruction);
    char mod = find_operation_mod(line);
    char* bin = translate_to_binary(oc, mod, line->reg1, line->reg1, line->reg2, 0);
    write_bytes_to_section(s, bin, INSTRUCTION_BYTE_LEN);
    return 0;
}
int handle_sh(s_asm_line* line, s_section* s)
{
    char oc = find_operation_code(line->instruction);
    char mod = find_operation_mod(line);
    char* bin = translate_to_binary(oc, mod, line->reg1, line->reg1, line->reg2, 0);
    write_bytes_to_section(s, bin, INSTRUCTION_BYTE_LEN);
    return 0;
}
int handle_ld(s_asm_line* line, s_section* s);
int handle_st(s_asm_line* line, s_section* s);
int handle_csrrd(s_asm_line* line, s_section* s);
int handle_csrwr(s_asm_line* line, s_section* s);

int handle_global(s_asm_line* line, s_section* s);
int handle_extern(s_asm_line* line, s_section* s);
int handle_s_section(s_asm_line* line, s_section* s);
int handle_word(s_asm_line* line, s_section* s);
int handle_skip(s_asm_line* line, s_section* s);
int handle_ascii(s_asm_line* line, s_section* s);
int handle_equ(s_asm_line* line, s_section* s);
int handle_end(s_asm_line* line, s_section* s);

int handle_label(s_asm_line* line, s_section* s);

char* translate_to_binary(char oc, char mod, char reg_a, char reg_b, char reg_c, long disp);

bool expr_is_valid(s_expr* expression, s_section* curr_section);
long calculate_expression(s_expr* expression);

bool label_fits_in_12b(char* label, s_section* curr_section);
long label_offset(char* label, s_section* curr_section);


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
        if (operation_to_oc[i][0] == line->instruction) {
            return operation_to_oc[i][1];
        }
    }
    return -1; // not found
}
