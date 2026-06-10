#include "code.h"

#include <stdio.h>

const char* asm_instruction_name(asm_instruction instruction)
{
    switch (instruction) {
    case ASM_INSTR_HALT: return "halt";
    case ASM_INSTR_INT: return "int";
    case ASM_INSTR_IRET: return "iret";
    case ASM_INSTR_CALL: return "call";
    case ASM_INSTR_RET: return "ret";
    case ASM_INSTR_JMP: return "jmp";
    case ASM_INSTR_BEQ: return "beq";
    case ASM_INSTR_BNE: return "bne";
    case ASM_INSTR_BGT: return "bgt";
    case ASM_INSTR_PUSH: return "push";
    case ASM_INSTR_POP: return "pop";
    case ASM_INSTR_XCHG: return "xchg";
    case ASM_INSTR_ADD: return "add";
    case ASM_INSTR_SUB: return "sub";
    case ASM_INSTR_MUL: return "mul";
    case ASM_INSTR_DIV: return "div";
    case ASM_INSTR_NOT: return "not";
    case ASM_INSTR_AND: return "and";
    case ASM_INSTR_OR: return "or";
    case ASM_INSTR_XOR: return "xor";
    case ASM_INSTR_SHL: return "shl";
    case ASM_INSTR_SHR: return "shr";
    case ASM_INSTR_LD: return "ld";
    case ASM_INSTR_ST: return "st";
    case ASM_INSTR_CSRRD: return "csrrd";
    case ASM_INSTR_CSRWR: return "csrwr";
    case ASM_INSTR_NONE: return NULL;
    }

    return NULL;
}

const char* asm_directive_name(asm_directive directive)
{
    switch (directive) {
    case ASM_DIR_GLOBAL: return "global";
    case ASM_DIR_EXTERN: return "extern";
    case ASM_DIR_SECTION: return "section";
    case ASM_DIR_WORD: return "word";
    case ASM_DIR_SKIP: return "skip";
    case ASM_DIR_ASCII: return "ascii";
    case ASM_DIR_EQU: return "equ";
    case ASM_DIR_END: return "end";
    case ASM_DIR_NONE: return NULL;
    }

    return NULL;
}

static void print_reg(asm_register reg)
{
    if (reg == ASM_REG_HANDLER) {
        printf("%%handler");
    } else if (reg == ASM_REG_STATUS) {
        printf("%%status");
    } else if (reg == ASM_REG_CAUSE) {
        printf("%%cause");
    } else if (reg == ASM_REG_SP) {
        printf("%%sp");
    } else if (reg == ASM_REG_PC) {
        printf("%%pc");
    } else if (reg >= ASM_REG_R0 && reg <= ASM_REG_R15) {
        printf("%%r%d", reg);
    } else {
        printf("%%?<%d>", reg);
    }
}

static void print_jmp_operand(const operand_jmp* operand)
{
    if (operand->is_literal) {
        printf("%ld", operand->literal);
    } else if (operand->is_symbol && operand->symbol) {
        printf("%s", operand->symbol);
    } else {
        printf("<missing operand>");
    }
}

static void print_ls_operand(const operand_ls* operand)
{
    if (operand->has_dollar) {
        printf("$");
    }

    if (operand->has_brackets) {
        printf("[");
    }

    if (operand->has_percent) {
        print_reg(operand->reg);
        if (operand->has_literal || operand->has_symbol) {
            printf(" + ");
        }
    }

    if (operand->has_literal) {
        printf("%ld", operand->literal);
    } else if (operand->has_symbol && operand->symbol) {
        printf("%s", operand->symbol);
    }

    if (operand->has_brackets) {
        printf("]");
    }
}

static void print_expr(const expr* expression)
{
    if (!expression) {
        printf("<missing expression>");
        return;
    }

    switch (expression->kind) {
    case EXPR_LITERAL:
        printf("%d", expression->literal);
        break;
    case EXPR_SYMBOL:
        printf("%s", expression->symbol ? expression->symbol : "<missing symbol>");
        break;
    case EXPR_NEG:
        printf("-");
        print_expr(expression->left);
        break;
    case EXPR_ADD:
    case EXPR_SUB:
    case EXPR_MUL:
    case EXPR_DIV:
        printf("(");
        print_expr(expression->left);
        printf(" %c ", expression->kind == EXPR_ADD ? '+'
                       : expression->kind == EXPR_SUB ? '-'
                       : expression->kind == EXPR_MUL ? '*'
                                                       : '/');
        print_expr(expression->right);
        printf(")");
        break;
    }
}

static void print_symbol_list(char** symbols, int symbol_count)
{
    for (int i = 0; i < symbol_count; i++) {
        if (i > 0) {
            printf(", ");
        }
        printf("%s", symbols[i] ? symbols[i] : "<missing symbol>");
    }
}

static void print_sym_or_lit_list(sym_or_lit** items, int item_count)
{
    for (int i = 0; i < item_count; i++) {
        sym_or_lit* item = items[i];

        if (i > 0) {
            printf(", ");
        }

        if (!item) {
            printf("<missing item>");
        } else if (item->is_literal) {
            printf("%d", item->literal);
        } else if (item->is_symbol && item->symbol) {
            printf("%s", item->symbol);
        } else {
            printf("<missing item>");
        }
    }
}

void print_asm_line(asm_line* line)
{
    if (!line) {
        printf("<null line>\n");
        return;
    }

    if (line->is_label) {
        printf("%s:\n", line->symbol ? line->symbol : "<missing label>");
        return;
    }

    if (line->is_directive) {
        const char* op = asm_directive_name(line->directive);
        if (op == NULL) {
            op = "<missing directive>";
        }

        printf(".%s", op);

        if (line->directive == ASM_DIR_GLOBAL || line->directive == ASM_DIR_EXTERN) {
            printf(" ");
            print_symbol_list(line->symbol_list, line->symbol_list_n);
        } else if (line->directive == ASM_DIR_SECTION) {
            printf(" %s", line->section_name ? line->section_name : "<missing section>");
        } else if (line->directive == ASM_DIR_WORD) {
            printf(" ");
            print_sym_or_lit_list(line->sym_or_lit_list, line->sym_or_lit_list_n);
        } else if (line->directive == ASM_DIR_SKIP) {
            printf(" %d", line->byte_num);
        } else if (line->directive == ASM_DIR_ASCII) {
            printf(" %s", line->ascii_string ? line->ascii_string : "<missing string>");
        } else if (line->directive == ASM_DIR_EQU) {
            printf(" %s, ", line->new_symbol ? line->new_symbol : "<missing symbol>");
            print_expr(&line->expression);
        }

        printf("\n");
        return;
    }

    if (line->is_instruction) {
        const char* op = asm_instruction_name(line->instruction);
        if (op == NULL) {
            op = "<missing instruction>";
        }

        printf("%s", op);

        if (line->instruction == ASM_INSTR_CALL || line->instruction == ASM_INSTR_JMP) {
            printf(" ");
            print_jmp_operand(&line->o_jmp);
        } else if (line->instruction == ASM_INSTR_BEQ ||
                   line->instruction == ASM_INSTR_BNE ||
                   line->instruction == ASM_INSTR_BGT) {
            printf(" ");
            print_reg(line->reg1);
            printf(", ");
            print_reg(line->reg2);
            printf(", ");
            print_jmp_operand(&line->o_jmp);
        } else if (line->instruction == ASM_INSTR_PUSH ||
                   line->instruction == ASM_INSTR_POP ||
                   line->instruction == ASM_INSTR_NOT) {
            printf(" ");
            print_reg(line->reg1);
        } else if (line->instruction == ASM_INSTR_XCHG ||
                   line->instruction == ASM_INSTR_ADD ||
                   line->instruction == ASM_INSTR_SUB ||
                   line->instruction == ASM_INSTR_MUL ||
                   line->instruction == ASM_INSTR_DIV ||
                   line->instruction == ASM_INSTR_AND ||
                   line->instruction == ASM_INSTR_OR ||
                   line->instruction == ASM_INSTR_XOR ||
                   line->instruction == ASM_INSTR_SHL ||
                   line->instruction == ASM_INSTR_SHR) {
            printf(" ");
            print_reg(line->reg1);
            printf(", ");
            print_reg(line->reg2);
        } else if (line->instruction == ASM_INSTR_LD) {
            printf(" ");
            print_ls_operand(&line->o_ls);
            printf(", ");
            print_reg(line->reg1);
        } else if (line->instruction == ASM_INSTR_ST) {
            printf(" ");
            print_reg(line->reg1);
            printf(", ");
            print_ls_operand(&line->o_ls);
        } else if (line->instruction == ASM_INSTR_CSRRD) {
            printf(" ");
            print_reg(line->reg1);
            printf(", ");
            print_reg(line->reg2);
        } else if (line->instruction == ASM_INSTR_CSRWR) {
            printf(" ");
            print_reg(line->reg1);
            printf(", ");
            print_reg(line->reg2);
        }

        printf("\n");
        return;
    }

    printf("<empty line>\n");
}
