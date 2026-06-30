#include "helper.hpp"
#include <cstdlib>
#include <cstring>
#include <iostream>

using namespace std;

extern vector<s_asm_line> lines;

static s_asm_instruction* parser_new_instruction(char instruction)
{
    s_asm_instruction* instr = new_asm_instruction();
    instr->instruction = instruction;
    return instr;
}

static void parser_apply_jump_operand(s_asm_instruction* instr, s_parser_jmp_operand* operand)
{
    instr->jump_branch_operand_type = operand->type;
    instr->jump_branch_literal = operand->literal;
    if (operand->symbol != NULL)
        instr->jump_branch_symbol = operand->symbol;
}

static void parser_apply_ls_operand(s_asm_instruction* instr, s_parser_ls_operand* operand)
{
    instr->load_store_operand_type = operand->type;
    instr->load_store_literal = operand->literal;
    instr->load_store_register = operand->reg;
    if (operand->symbol != NULL)
        instr->load_store_symbol = operand->symbol;
}

char reg_from_name(char* reg)
{
    string reg_name = reg;

    if (reg_name == "sp")
        return ASM_REG_SP;
    if (reg_name == "pc")
        return ASM_REG_PC;
    if (reg_name[0] == 'r')
        return stoi(reg_name.substr(1));

    if (reg_name == "cause")
        return ASM_REG_CAUSE;
    if (reg_name == "handler")
        return ASM_REG_HANDLER;
    if (reg_name == "status")
        return ASM_REG_STATUS;

    return -1;
}

s_sym_lit new_lit_sl(long lit)
{
    s_sym_lit new_lit;
    new_lit.literal = lit;
    new_lit.type = SL_LIT;
    return new_lit;
}

s_sym_lit new_sym_sl(char* sym)
{
    s_sym_lit new_lit;
    new_lit.symbol = sym;
    new_lit.type = SL_SYM;
    return new_lit;
}

long parse_literal_lex(char* lit)
{
    string literal_string = lit;
    if (literal_string.size() > 1 && literal_string[1] == 'x')
        return stol(literal_string, nullptr, 16);
    else
        return stol(literal_string, nullptr, 10);
}

char* parser_copy_symbol(const char* symbol)
{
    size_t len = strlen(symbol);
    char* copy = (char*)malloc(len + 1);
    memcpy(copy, symbol, len + 1);
    return copy;
}

s_symbol_list* parser_new_symbol_list(char* symbol)
{
    s_symbol_list* list = new s_symbol_list();
    parser_add_symbol_to_list(list, symbol);
    return list;
}

void parser_add_symbol_to_list(s_symbol_list* list, char* symbol)
{
    list->push_back(symbol);
}

s_sym_lit_list* parser_new_sym_lit_list(s_sym_lit sym_lit)
{
    s_sym_lit_list* list = new s_sym_lit_list();
    parser_add_sym_lit_to_list(list, sym_lit);
    return list;
}

void parser_add_sym_lit_to_list(s_sym_lit_list* list, s_sym_lit sym_lit)
{
    list->push_back(sym_lit);
}

s_parser_jmp_operand* parser_new_jmp_literal(long literal)
{
    s_parser_jmp_operand* operand = new s_parser_jmp_operand();
    operand->type = ASM_OP_JMP_LIT;
    operand->literal = literal;
    operand->symbol = NULL;
    return operand;
}

s_parser_jmp_operand* parser_new_jmp_symbol(char* symbol)
{
    s_parser_jmp_operand* operand = new s_parser_jmp_operand();
    operand->type = ASM_OP_JMP_SYM;
    operand->literal = 0;
    operand->symbol = symbol;
    return operand;
}

s_parser_ls_operand* parser_new_ls_operand(char type, long literal, char* symbol, char reg)
{
    s_parser_ls_operand* operand = new s_parser_ls_operand();
    operand->type = type;
    operand->literal = literal;
    operand->symbol = symbol;
    operand->reg = reg;
    return operand;
}

void parser_add_label(char* label)
{
    add_label_to_program(lines, label);
}

void parser_add_symbol_list_directive(char directive, s_symbol_list* symbols)
{
    s_asm_directive* dir = new_asm_directive();
    dir->directive = directive;
    dir->symbol_list = *symbols;
    add_directive_to_program(lines, dir);
}

void parser_add_section_directive(char* section)
{
    s_asm_directive* dir = new_asm_directive();
    dir->directive = ASM_DIR_SECTION;
    dir->section_symbol = section;
    add_directive_to_program(lines, dir);
}

void parser_add_word_directive(s_sym_lit_list* sym_lits)
{
    s_asm_directive* dir = new_asm_directive();
    dir->directive = ASM_DIR_WORD;
    dir->sym_lit_list = *sym_lits;
    add_directive_to_program(lines, dir);
}

void parser_add_skip_directive(long literal)
{
    s_asm_directive* dir = new_asm_directive();
    dir->directive = ASM_DIR_SKIP;
    dir->skip_literal = literal;
    add_directive_to_program(lines, dir);
}

void parser_add_ascii_directive(char* ascii)
{
    s_asm_directive* dir = new_asm_directive();
    dir->directive = ASM_DIR_ASCII;
    dir->ascii_string = ascii;
    add_directive_to_program(lines, dir);
}

void parser_add_equ_directive(char* symbol, s_expr* expr)
{
    s_asm_directive* dir = new_asm_directive();
    dir->directive = ASM_DIR_EQU;
    dir->equ_symbol = symbol;
    dir->expr = expr;
    add_directive_to_program(lines, dir);
}

void parser_add_end_directive()
{
    s_asm_directive* dir = new_asm_directive();
    dir->directive = ASM_DIR_END;
    add_directive_to_program(lines, dir);
}

void parser_add_no_operand_instruction(char instruction)
{
    add_instruction_to_program(lines, parser_new_instruction(instruction));
}

void parser_add_jump_instruction(char instruction, s_parser_jmp_operand* operand)
{
    s_asm_instruction* instr = parser_new_instruction(instruction);
    parser_apply_jump_operand(instr, operand);
    add_instruction_to_program(lines, instr);
}

void parser_add_branch_instruction(char instruction, s_gpr_pair regs, s_parser_jmp_operand* operand)
{
    s_asm_instruction* instr = parser_new_instruction(instruction);
    instr->reg1 = regs.reg1;
    instr->reg2 = regs.reg2;
    parser_apply_jump_operand(instr, operand);
    add_instruction_to_program(lines, instr);
}

void parser_add_single_gpr_instruction(char instruction, char reg)
{
    s_asm_instruction* instr = parser_new_instruction(instruction);
    instr->reg1 = reg;
    add_instruction_to_program(lines, instr);
}

void parser_add_two_gpr_instruction(char instruction, s_gpr_pair regs)
{
    s_asm_instruction* instr = parser_new_instruction(instruction);
    instr->reg1 = regs.reg1;
    instr->reg2 = regs.reg2;
    add_instruction_to_program(lines, instr);
}

void parser_add_ld_instruction(s_parser_ls_operand* operand, char reg)
{
    s_asm_instruction* instr = parser_new_instruction(ASM_INSTR_LD);
    instr->reg1 = reg;
    parser_apply_ls_operand(instr, operand);
    add_instruction_to_program(lines, instr);
}

void parser_add_st_instruction(char reg, s_parser_ls_operand* operand)
{
    s_asm_instruction* instr = parser_new_instruction(ASM_INSTR_ST);
    instr->reg1 = reg;
    parser_apply_ls_operand(instr, operand);
    add_instruction_to_program(lines, instr);
}

void parser_add_csrrd_instruction(char csr, char reg)
{
    s_asm_instruction* instr = parser_new_instruction(ASM_INSTR_CSRRD);
    instr->control_reg = csr;
    instr->reg2 = reg;
    add_instruction_to_program(lines, instr);
}

void parser_add_csrwr_instruction(char reg, char csr)
{
    s_asm_instruction* instr = parser_new_instruction(ASM_INSTR_CSRWR);
    instr->reg1 = reg;
    instr->control_reg = csr;
    add_instruction_to_program(lines, instr);
}

void yyerror(const char *message)
{
    cout << "Parser Error: " << message << endl;
}
