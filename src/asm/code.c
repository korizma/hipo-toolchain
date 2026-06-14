#include "code.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* asm_instruction_name(e_asm_instruction instruction)
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

const char* asm_directive_name(e_asm_directive directive)
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

typedef struct
{
    char* data;
    size_t length;
    size_t capacity;
    bool failed;
} s_string_builder;

static char* duplicate_string(const char* string)
{
    if (string == NULL)
        string = "";

    size_t length = strlen(string) + 1;
    char* copy = (char*)malloc(length);
    if (copy == NULL)
        return NULL;

    memcpy(copy, string, length);
    return copy;
}

static char* format_to_string(const char* format, ...)
{
    va_list args;
    va_list args_copy;

    va_start(args, format);
    va_copy(args_copy, args);
    int required = vsnprintf(NULL, 0, format, args_copy);
    va_end(args_copy);

    if (required < 0) {
        va_end(args);
        return NULL;
    }

    char* result = (char*)malloc((size_t)required + 1);
    if (result == NULL) {
        va_end(args);
        return NULL;
    }

    vsnprintf(result, (size_t)required + 1, format, args);
    va_end(args);
    return result;
}

static void string_builder_init(s_string_builder* builder)
{
    builder->length = 0;
    builder->capacity = 64;
    builder->data = (char*)malloc(builder->capacity);
    builder->failed = builder->data == NULL;

    if (!builder->failed)
        builder->data[0] = '\0';
}

static void string_builder_reserve(s_string_builder* builder, size_t additional)
{
    if (builder->failed)
        return;

    size_t required = builder->length + additional + 1;
    if (required <= builder->capacity)
        return;

    size_t new_capacity = builder->capacity;
    while (new_capacity < required)
        new_capacity *= 2;

    char* data = (char*)realloc(builder->data, new_capacity);
    if (data == NULL) {
        builder->failed = true;
        return;
    }

    builder->data = data;
    builder->capacity = new_capacity;
}

static void string_builder_append(s_string_builder* builder, const char* string)
{
    if (string == NULL)
        string = "";

    size_t length = strlen(string);
    string_builder_reserve(builder, length);
    if (builder->failed)
        return;

    memcpy(builder->data + builder->length, string, length + 1);
    builder->length += length;
}

static void string_builder_appendf(s_string_builder* builder, const char* format, ...)
{
    if (builder->failed)
        return;

    va_list args;
    va_list args_copy;

    va_start(args, format);
    va_copy(args_copy, args);
    int required = vsnprintf(NULL, 0, format, args_copy);
    va_end(args_copy);

    if (required < 0) {
        builder->failed = true;
        va_end(args);
        return;
    }

    string_builder_reserve(builder, (size_t)required);
    if (builder->failed) {
        va_end(args);
        return;
    }

    vsnprintf(builder->data + builder->length,
              builder->capacity - builder->length,
              format,
              args);
    builder->length += (size_t)required;
    va_end(args);
}

static void string_builder_append_string(s_string_builder* builder, char* string)
{
    if (string == NULL) {
        builder->failed = true;
        return;
    }

    string_builder_append(builder, string);
    free(string);
}

static char* string_builder_take(s_string_builder* builder)
{
    if (builder->failed) {
        free(builder->data);
        return NULL;
    }

    return builder->data;
}

static char* reg_to_string(e_asm_register reg)
{
    if (reg == ASM_REG_HANDLER) {
        return duplicate_string("%handler");
    } else if (reg == ASM_REG_STATUS) {
        return duplicate_string("%status");
    } else if (reg == ASM_REG_CAUSE) {
        return duplicate_string("%cause");
    } else if (reg == ASM_REG_SP) {
        return duplicate_string("%sp");
    } else if (reg == ASM_REG_PC) {
        return duplicate_string("%pc");
    } else if (reg >= ASM_REG_R0 && reg <= ASM_REG_R15) {
        return format_to_string("%%r%d", reg);
    } else {
        return format_to_string("%%?<%d>", reg);
    }
}

static char* jmp_operand_to_string(const s_operand_jmp* operand)
{
    if (operand->is_literal) {
        return format_to_string("%ld", operand->literal);
    } else if (operand->is_symbol && operand->symbol) {
        return duplicate_string(operand->symbol);
    } else {
        return duplicate_string("<missing operand>");
    }
}

static char* ls_operand_to_string(const s_operand_ls* operand)
{
    switch (operand->kind) {
    case ASM_OPERAND_LS_IMM_LITERAL:
        return format_to_string("$%ld", operand->literal);
    case ASM_OPERAND_LS_IMM_SYMBOL:
        return format_to_string("$%s", operand->symbol ? operand->symbol : "<missing symbol>");
    case ASM_OPERAND_LS_MEM_LITERAL:
        return format_to_string("%ld", operand->literal);
    case ASM_OPERAND_LS_MEM_SYMBOL:
        return duplicate_string(operand->symbol ? operand->symbol : "<missing symbol>");
    case ASM_OPERAND_LS_REG:
        return reg_to_string(operand->reg);
    case ASM_OPERAND_LS_REG_INDIRECT: {
        char* reg = reg_to_string(operand->reg);
        char* result = format_to_string("[%s]", reg ? reg : "<missing register>");
        free(reg);
        return result;
    }
    case ASM_OPERAND_LS_REG_INDIRECT_LITERAL: {
        char* reg = reg_to_string(operand->reg);
        char* result = format_to_string("[%s + %ld]", reg ? reg : "<missing register>", operand->literal);
        free(reg);
        return result;
    }
    case ASM_OPERAND_LS_REG_INDIRECT_SYMBOL: {
        char* reg = reg_to_string(operand->reg);
        char* result = format_to_string("[%s + %s]",
                                        reg ? reg : "<missing register>",
                                        operand->symbol ? operand->symbol : "<missing symbol>");
        free(reg);
        return result;
    }
    case ASM_OPERAND_LS_NONE:
        return duplicate_string("<missing operand>");
    }

    return duplicate_string("<missing operand>");
}

static char* expr_to_string(const s_expr* expression)
{
    if (!expression) {
        return duplicate_string("<missing expression>");
    }

    s_string_builder builder;
    string_builder_init(&builder);

    bool has_term = false;
    for (int i = 0; i < expression->symbol_num; i++) {
        int coefficient = expression->symbol_coeff[i];
        if (coefficient == 0) {
            continue;
        }

        const char* symbol = expression->symbol_list[i] ? expression->symbol_list[i] : "<missing symbol>";
        int magnitude = coefficient < 0 ? -coefficient : coefficient;

        if (!has_term) {
            if (coefficient < 0) {
                string_builder_append(&builder, "-");
            }
        } else {
            string_builder_append(&builder, coefficient < 0 ? " - " : " + ");
        }

        if (magnitude == 1) {
            string_builder_append(&builder, symbol);
        } else {
            string_builder_appendf(&builder, "%d * %s", magnitude, symbol);
        }

        has_term = true;
    }

    if (expression->value != 0 || !has_term) {
        if (!has_term) {
            string_builder_appendf(&builder, "%ld", expression->value);
        } else if (expression->value > 0) {
            string_builder_appendf(&builder, " + %ld", expression->value);
        } else {
            unsigned long magnitude = (unsigned long)(-(expression->value + 1)) + 1;
            string_builder_appendf(&builder, " - %lu", magnitude);
        }
    }

    return string_builder_take(&builder);
}

static char* symbol_list_to_string(char** symbols, int symbol_count)
{
    s_string_builder builder;
    string_builder_init(&builder);

    for (int i = 0; i < symbol_count; i++) {
        if (i > 0) {
            string_builder_append(&builder, ", ");
        }
        string_builder_append(&builder, symbols[i] ? symbols[i] : "<missing symbol>");
    }

    return string_builder_take(&builder);
}

static char* sym_or_lit_list_to_string(s_sym_or_lit** items, int item_count)
{
    s_string_builder builder;
    string_builder_init(&builder);

    for (int i = 0; i < item_count; i++) {
        s_sym_or_lit* item = items[i];

        if (i > 0) {
            string_builder_append(&builder, ", ");
        }

        if (!item) {
            string_builder_append(&builder, "<missing item>");
        } else if (item->is_literal) {
            string_builder_appendf(&builder, "%ld", item->literal);
        } else if (item->is_symbol && item->symbol) {
            string_builder_append(&builder, item->symbol);
        } else {
            string_builder_append(&builder, "<missing item>");
        }
    }

    return string_builder_take(&builder);
}

char* asm_line_to_string(s_asm_line* line)
{
    if (!line) {
        return duplicate_string("<null line>\n");
    }

    if (line->is_label) {
        return format_to_string("%s:\n", line->symbol ? line->symbol : "<missing label>");
    }

    if (line->is_directive) {
        s_string_builder builder;
        string_builder_init(&builder);

        const char* op = asm_directive_name(line->directive);
        if (op == NULL) {
            op = "<missing directive>";
        }

        string_builder_appendf(&builder, ".%s", op);

        if (line->directive == ASM_DIR_GLOBAL || line->directive == ASM_DIR_EXTERN) {
            string_builder_append(&builder, " ");
            string_builder_append_string(&builder, symbol_list_to_string(line->symbol_list, line->symbol_list_n));
        } else if (line->directive == ASM_DIR_SECTION) {
            string_builder_appendf(&builder, " %s", line->section_name ? line->section_name : "<missing section>");
        } else if (line->directive == ASM_DIR_WORD) {
            string_builder_append(&builder, " ");
            string_builder_append_string(&builder, sym_or_lit_list_to_string(line->sym_or_lit_list, line->sym_or_lit_list_n));
        } else if (line->directive == ASM_DIR_SKIP) {
            string_builder_appendf(&builder, " %d", line->byte_num);
        } else if (line->directive == ASM_DIR_ASCII) {
            string_builder_appendf(&builder, " %s", line->ascii_string ? line->ascii_string : "<missing string>");
        } else if (line->directive == ASM_DIR_EQU) {
            string_builder_appendf(&builder, " %s, ", line->new_symbol ? line->new_symbol : "<missing symbol>");
            string_builder_append_string(&builder, expr_to_string(line->expression));
        }

        string_builder_append(&builder, "\n");
        return string_builder_take(&builder);
    }

    if (line->is_instruction) {
        s_string_builder builder;
        string_builder_init(&builder);

        const char* op = asm_instruction_name(line->instruction);
        if (op == NULL) {
            op = "<missing instruction>";
        }

        string_builder_append(&builder, op);

        if (line->instruction == ASM_INSTR_CALL || line->instruction == ASM_INSTR_JMP) {
            string_builder_append(&builder, " ");
            string_builder_append_string(&builder, jmp_operand_to_string(&line->o_jmp));
        } else if (line->instruction == ASM_INSTR_BEQ ||
                   line->instruction == ASM_INSTR_BNE ||
                   line->instruction == ASM_INSTR_BGT) {
            string_builder_append(&builder, " ");
            string_builder_append_string(&builder, reg_to_string(line->reg1));
            string_builder_append(&builder, ", ");
            string_builder_append_string(&builder, reg_to_string(line->reg2));
            string_builder_append(&builder, ", ");
            string_builder_append_string(&builder, jmp_operand_to_string(&line->o_jmp));
        } else if (line->instruction == ASM_INSTR_PUSH ||
                   line->instruction == ASM_INSTR_POP ||
                   line->instruction == ASM_INSTR_NOT) {
            string_builder_append(&builder, " ");
            string_builder_append_string(&builder, reg_to_string(line->reg1));
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
            string_builder_append(&builder, " ");
            string_builder_append_string(&builder, reg_to_string(line->reg1));
            string_builder_append(&builder, ", ");
            string_builder_append_string(&builder, reg_to_string(line->reg2));
        } else if (line->instruction == ASM_INSTR_LD) {
            string_builder_append(&builder, " ");
            string_builder_append_string(&builder, ls_operand_to_string(&line->o_ls));
            string_builder_append(&builder, ", ");
            string_builder_append_string(&builder, reg_to_string(line->reg1));
        } else if (line->instruction == ASM_INSTR_ST) {
            string_builder_append(&builder, " ");
            string_builder_append_string(&builder, reg_to_string(line->reg1));
            string_builder_append(&builder, ", ");
            string_builder_append_string(&builder, ls_operand_to_string(&line->o_ls));
        } else if (line->instruction == ASM_INSTR_CSRRD) {
            string_builder_append(&builder, " ");
            string_builder_append_string(&builder, reg_to_string(line->reg1));
            string_builder_append(&builder, ", ");
            string_builder_append_string(&builder, reg_to_string(line->reg2));
        } else if (line->instruction == ASM_INSTR_CSRWR) {
            string_builder_append(&builder, " ");
            string_builder_append_string(&builder, reg_to_string(line->reg1));
            string_builder_append(&builder, ", ");
            string_builder_append_string(&builder, reg_to_string(line->reg2));
        }

        string_builder_append(&builder, "\n");
        return string_builder_take(&builder);
    }

    return duplicate_string("<empty line>\n");
}
