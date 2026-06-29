#pragma once

#include "../c_src/asm_directive.hpp"
#include "../c_src/asm_instruction.hpp"
#include "../c_src/asm.hpp"
#include "../c_src/expr.hpp"
#include <vector>

typedef struct {
    char reg1;
    char reg2;
} s_gpr_pair;

typedef std::vector<std::string> s_symbol_list;
typedef std::vector<s_sym_lit> s_sym_lit_list;

typedef struct {
    char type;
    long literal;
    char* symbol;
} s_parser_jmp_operand;

typedef struct {
    char type;
    long literal;
    char* symbol;
    char reg;
} s_parser_ls_operand;

char reg_from_name(char* reg);

s_sym_lit new_lit_sl(long lit);
s_sym_lit new_sym_sl(char* sym);

long parse_literal_lex(char* lit);
char* parser_copy_symbol(const char* symbol);

s_symbol_list* parser_new_symbol_list(char* symbol);
void parser_add_symbol_to_list(s_symbol_list* list, char* symbol);

s_sym_lit_list* parser_new_sym_lit_list(s_sym_lit sym_lit);
void parser_add_sym_lit_to_list(s_sym_lit_list* list, s_sym_lit sym_lit);

s_parser_jmp_operand* parser_new_jmp_literal(long literal);
s_parser_jmp_operand* parser_new_jmp_symbol(char* symbol);
s_parser_ls_operand* parser_new_ls_operand(char type, long literal, char* symbol, char reg);

void parser_add_label(char* label);
void parser_add_symbol_list_directive(char directive, s_symbol_list* symbols);
void parser_add_section_directive(char* section);
void parser_add_word_directive(s_sym_lit_list* sym_lits);
void parser_add_skip_directive(long literal);
void parser_add_ascii_directive(char* ascii);
void parser_add_equ_directive(char* symbol, s_expr* expr);
void parser_add_end_directive();

void parser_add_no_operand_instruction(char instruction);
void parser_add_jump_instruction(char instruction, s_parser_jmp_operand* operand);
void parser_add_branch_instruction(char instruction, s_gpr_pair regs, s_parser_jmp_operand* operand);
void parser_add_single_gpr_instruction(char instruction, char reg);
void parser_add_two_gpr_instruction(char instruction, s_gpr_pair regs);
void parser_add_ld_instruction(s_parser_ls_operand* operand, char reg);
void parser_add_st_instruction(char reg, s_parser_ls_operand* operand);
void parser_add_csrrd_instruction(char csr, char reg);
void parser_add_csrwr_instruction(char reg, char csr);

void yyerror(const char *message);
