#include "code.h"

#include <stdio.h>
#include <string.h>

static void print_reg(int reg)
{
    if (reg == 14) {
        printf("%%sp");
    } else if (reg == 15) {
        printf("%%pc");
    } else if (reg >= 0) {
        printf("%%r%d", reg);
    } else if (reg == -1) {
        printf("%%handler");
    } else if (reg == -2) {
        printf("%%status");
    } else if (reg == -3) {
        printf("%%cause");
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
        const char* op = line->operation ? line->operation : "<missing directive>";

        printf(".%s", op);

        if (strcmp(op, "global") == 0 || strcmp(op, "extern") == 0) {
            printf(" ");
            print_symbol_list(line->symbol_list, line->symbol_list_n);
        } else if (strcmp(op, "section") == 0) {
            printf(" %s", line->section_name ? line->section_name : "<missing section>");
        } else if (strcmp(op, "word") == 0) {
            printf(" ");
            print_sym_or_lit_list(line->sym_or_lit_list, line->sym_or_lit_list_n);
        } else if (strcmp(op, "skip") == 0) {
            printf(" %d", line->byte_num);
        } else if (strcmp(op, "ascii") == 0) {
            printf(" %s", line->ascii_string ? line->ascii_string : "<missing string>");
        } else if (strcmp(op, "equ") == 0) {
            printf(" %s, ", line->new_symbol ? line->new_symbol : "<missing symbol>");
            print_expr(&line->expression);
        }

        printf("\n");
        return;
    }

    if (line->is_instruction) {
        const char* op = line->operation ? line->operation : "<missing instruction>";

        printf("%s", op);

        if (strcmp(op, "call") == 0 || strcmp(op, "jmp") == 0) {
            printf(" ");
            print_jmp_operand(&line->o_jmp);
        } else if (strcmp(op, "beq") == 0 || strcmp(op, "bne") == 0 ||
                   strcmp(op, "bgt") == 0) {
            printf(" ");
            print_reg(line->reg1);
            printf(", ");
            print_reg(line->reg2);
            printf(", ");
            print_jmp_operand(&line->o_jmp);
        } else if (strcmp(op, "push") == 0 || strcmp(op, "pop") == 0 ||
                   strcmp(op, "not") == 0) {
            printf(" ");
            print_reg(line->reg1);
        } else if (strcmp(op, "xchg") == 0 || strcmp(op, "add") == 0 ||
                   strcmp(op, "sub") == 0 || strcmp(op, "mul") == 0 ||
                   strcmp(op, "div") == 0 || strcmp(op, "and") == 0 ||
                   strcmp(op, "or") == 0 || strcmp(op, "xor") == 0 ||
                   strcmp(op, "shl") == 0 || strcmp(op, "shr") == 0) {
            printf(" ");
            print_reg(line->reg1);
            printf(", ");
            print_reg(line->reg2);
        } else if (strcmp(op, "ld") == 0) {
            printf(" ");
            print_ls_operand(&line->o_ls);
            printf(", ");
            print_reg(line->reg1);
        } else if (strcmp(op, "st") == 0) {
            printf(" ");
            print_reg(line->reg1);
            printf(", ");
            print_ls_operand(&line->o_ls);
        } else if (strcmp(op, "csrrd") == 0) {
            printf(" ");
            print_reg(line->reg1);
            printf(", ");
            print_reg(line->reg2);
        } else if (strcmp(op, "csrwr") == 0) {
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
