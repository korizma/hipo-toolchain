#include "error.h"

#include <stdio.h>
#include <stdlib.h>

static const char* operand_kind_name(e_operand_ls_kind kind)
{
    switch (kind)
    {
    case ASM_OPERAND_LS_IMM_LITERAL: return "IMM_LITERAL";
    case ASM_OPERAND_LS_IMM_SYMBOL: return "IMM_SYMBOL";
    case ASM_OPERAND_LS_MEM_LITERAL: return "MEM_LITERAL";
    case ASM_OPERAND_LS_MEM_SYMBOL: return "MEM_SYMBOL";
    case ASM_OPERAND_LS_REG: return "REG";
    case ASM_OPERAND_LS_REG_INDIRECT: return "REG_INDIRECT";
    case ASM_OPERAND_LS_REG_INDIRECT_LITERAL: return "REG_INDIRECT_LITERAL";
    case ASM_OPERAND_LS_REG_INDIRECT_SYMBOL: return "REG_INDIRECT_SYMBOL";
    case ASM_OPERAND_LS_NONE: return "NONE";
    }

    return "UNKNOWN";
}

static const char* line_instruction_name(s_asm_line* line)
{
    const char* name = line != 0 ? asm_instruction_name(line->instruction) : 0;
    return name != 0 ? name : "<unknown>";
}

static const char* line_directive_name(s_asm_line* line)
{
    const char* name = line != 0 ? asm_directive_name(line->directive) : 0;
    return name != 0 ? name : "<unknown>";
}

static const char* error_symbol(s_error* err)
{
    if (err->symbol != 0)
        return err->symbol;

    if (err->line != 0)
    {
        if (err->line->symbol != 0)
            return err->line->symbol;
        if (err->line->section_name != 0)
            return err->line->section_name;
    }

    return "<unknown>";
}

s_error* new_error(s_asm_line* line, e_error_type type)
{
    s_error* err = malloc(sizeof(s_error));
    err->type = type;
    err->line = line;
    err->symbol = 0;
    err->literal = 0;
    err->operand_kind = ASM_OPERAND_LS_NONE;
    return err;
}

s_error* new_symbol_error(s_asm_line* line, e_error_type type, const char* symbol)
{
    s_error* err = new_error(line, type);
    err->symbol = symbol;
    return err;
}

s_error* new_literal_error(s_asm_line* line, e_error_type type, long literal)
{
    s_error* err = new_error(line, type);
    err->literal = literal;
    return err;
}

s_error* new_operand_error(s_asm_line* line, e_error_type type, e_operand_ls_kind operand_kind)
{
    s_error* err = new_error(line, type);
    err->operand_kind = operand_kind;
    return err;
}

s_error* new_operand_literal_error(s_asm_line* line, e_error_type type, e_operand_ls_kind operand_kind, long literal)
{
    s_error* err = new_literal_error(line, type, literal);
    err->operand_kind = operand_kind;
    return err;
}

void print_error(s_error* err)
{
    if (err == 0)
        return;

    printf("ERROR: ");
    switch (err->type)
    {
    case ERR_OUTSIDE_SECTION:
        if (err->line != 0 && err->line->is_instruction)
            printf("Instruction %s defined outside of a section!\n", line_instruction_name(err->line));
        else if (err->line != 0 && err->line->is_directive)
            printf("Directive .%s used outside of a section!\n", line_directive_name(err->line));
        else if (err->line != 0 && err->line->is_label)
            printf("Label %s defined outside of a section!\n", error_symbol(err));
        else
            printf("Line defined outside of a section!\n");
        break;
    case ERR_DUPLICATE_SYMBOL:
        printf("symbol %s already defined!\n", error_symbol(err));
        break;
    case ERR_DUPLICATE_SECTION:
        printf("Section %s already defined!\n", error_symbol(err));
        break;
    case ERR_INVALID_EXTERN_SYMBOL:
        printf("symbol %s already defined, cannot be declared as an extern symbol!\n", error_symbol(err));
        break;
    case ERR_INVALID_GLOBAL_SYMBOL:
        printf("Symbol %s is a section, it cannot be declared as a global symbol!\n", error_symbol(err));
        break;
    case ERR_SECTION_AS_DESTINATION:
        printf("Instruction %s used section as a destination!\n", line_instruction_name(err->line));
        break;
    case ERR_INVALID_OPERAND:
        printf("Instruction %s used invalid operand %s!\n",
               line_instruction_name(err->line),
               operand_kind_name(err->operand_kind));
        break;
    case ERR_UNSUPPORTED_OPERAND:
        printf("Instruction %s does not support operand %s!\n",
               line_instruction_name(err->line),
               operand_kind_name(err->operand_kind));
        break;
    case ERR_LITERAL_OUT_OF_RANGE_12B:
        printf("Instruction %s operand %s literal %ld is outside signed 12-bit range!\n",
               line_instruction_name(err->line),
               operand_kind_name(err->operand_kind),
               err->literal);
        break;
    case ERR_MISSING_HANDLER:
        printf("No handler registered for line!\n");
        break;
    case ERR_INVALID_JUMP_OPERAND:
        printf("Instruction %s has invalid jump operand!\n", line_instruction_name(err->line));
        break;
    case ERR_UNRESOLVED_SYMBOL:
        printf("symbol '%s' is referenced but not defined\n", error_symbol(err));
        break;
    }

    if (err->line != 0)
    {
        char* line_str = asm_line_to_string(err->line);
        if (line_str != 0)
        {
            printf("Line %d: %s", err->line->line_num, line_str);
            free(line_str);
        }
    }
}

void free_error(s_error* err)
{
    free(err);
}
