#include "printing.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>

static char* string_to_c_string(const string& value)
{
    char* result = (char*)malloc(value.size() + 1);
    if (result == NULL)
        return NULL;

    memcpy(result, value.c_str(), value.size() + 1);
    return result;
}

static string nullable_string(const char* value, const string& fallback)
{
    return value != NULL ? string(value) : fallback;
}

static string owned_c_string_to_string(char* value, const string& fallback)
{
    if (value == NULL)
        return fallback;

    string result = value;
    free(value);
    return result;
}

static string register_to_string(e_asm_register reg)
{
    if (reg == ASM_REG_HANDLER)
        return "%handler";
    if (reg == ASM_REG_STATUS)
        return "%status";
    if (reg == ASM_REG_CAUSE)
        return "%cause";
    if (reg == ASM_REG_SP)
        return "%sp";
    if (reg == ASM_REG_PC)
        return "%pc";
    if (reg >= ASM_REG_R0 && reg <= ASM_REG_R15)
        return "%r" + to_string(reg);

    return "%?<" + to_string(reg) + ">";
}

static string jump_operand_to_string(const s_operand_jmp& operand)
{
    if (operand.is_literal)
        return to_string(operand.literal);
    if (operand.is_symbol)
        return nullable_string(operand.symbol, "<missing symbol>");

    return "<missing operand>";
}

static string load_store_operand_to_string(const s_operand_ls& operand)
{
    switch (operand.kind)
    {
        case ASM_OPERAND_LS_IMM_LITERAL:
            return "$" + to_string(operand.literal);
        case ASM_OPERAND_LS_IMM_SYMBOL:
            return "$" + nullable_string(operand.symbol, "<missing symbol>");
        case ASM_OPERAND_LS_MEM_LITERAL:
            return to_string(operand.literal);
        case ASM_OPERAND_LS_MEM_SYMBOL:
            return nullable_string(operand.symbol, "<missing symbol>");
        case ASM_OPERAND_LS_REG:
            return register_to_string(operand.reg);
        case ASM_OPERAND_LS_REG_INDIRECT:
            return "[" + register_to_string(operand.reg) + "]";
        case ASM_OPERAND_LS_REG_INDIRECT_LITERAL:
            return "[" + register_to_string(operand.reg) + " + " + to_string(operand.literal) + "]";
        case ASM_OPERAND_LS_REG_INDIRECT_SYMBOL:
            return "[" + register_to_string(operand.reg) + " + " + nullable_string(operand.symbol, "<missing symbol>") + "]";
        case ASM_OPERAND_LS_NONE:
            return "<missing operand>";
    }

    return "<missing operand>";
}

static string expression_to_string(const s_expr* expression)
{
    if (expression == NULL)
        return "<missing expression>";

    string result = "";
    bool has_term = false;

    for (int i = 0; i < expression->symbol_num; i++)
    {
        int coefficient = expression->symbol_coeff[i];
        if (coefficient == 0)
            continue;

        string symbol = nullable_string(expression->symbol_list[i], "<missing symbol>");
        int magnitude = coefficient < 0 ? -coefficient : coefficient;

        if (!has_term)
        {
            if (coefficient < 0)
                result += "-";
        }
        else
        {
            result += coefficient < 0 ? " - " : " + ";
        }

        if (magnitude != 1)
            result += to_string(magnitude) + " * ";
        result += symbol;
        has_term = true;
    }

    if (expression->value != 0 || !has_term)
    {
        if (!has_term)
        {
            result += to_string(expression->value);
        }
        else if (expression->value > 0)
        {
            result += " + " + to_string(expression->value);
        }
        else
        {
            result += " - " + to_string(-expression->value);
        }
    }

    return result;
}

static string symbol_list_to_string(char** symbols, int symbol_count)
{
    string result = "";
    for (int i = 0; i < symbol_count; i++)
    {
        if (i > 0)
            result += ", ";
        result += nullable_string(symbols[i], "<missing symbol>");
    }
    return result;
}

static string sym_or_lit_list_to_string(s_sym_or_lit** items, int item_count)
{
    string result = "";
    for (int i = 0; i < item_count; i++)
    {
        if (i > 0)
            result += ", ";

        s_sym_or_lit* item = items[i];
        if (item == NULL)
            result += "<missing item>";
        else if (item->is_literal)
            result += to_string(item->literal);
        else if (item->is_symbol)
            result += nullable_string(item->symbol, "<missing symbol>");
        else
            result += "<missing item>";
    }
    return result;
}

char* program_to_string(s_program *program)
{
    string final_string = "";
    string sym_table = symbol_table_to_string(program->sym_table);

    final_string += sym_table;

    for (int i = 0; i < program->number_of_sections; i++)
    {
        string section_string = section_to_string(program->sections[i]);
        final_string += section_string;
        if (program->sections[i]->rela_table)
            final_string += rela_table_to_string(program->sections[i]->rela_table);
    }

    char* result = (char*)malloc(final_string.size() + 1);

    memcpy(result, final_string.c_str(), final_string.size() + 1); // includes '\0'
    return result;
}

extern "C" void print_program()
{
    char* program = program_to_string(&p);
    if (program == NULL)
        return;

    printf("%s", program);
    free(program);
}

string section_to_string(s_section *section)
{
    string final_string = "";

    final_string += "#";
    final_string += section->name;
    final_string += "\n";

    for (int i = 0; i < section->next_free; i++)
    {
        string hex = char_to_hex_string(section->bytes[i]);

        final_string += hex;
        final_string += " ";

        if (i % 8 == 3)
            final_string += "  ";

        if (i % 8 == 7)
            final_string += "\n";
    }

    final_string += "\n";

    return final_string;
}

string symbol_table_to_string(s_symbol_table *sym_table)
{
    string final_string = "#.symtab\n";

    final_string += "Num\tValue\tSize\tType\tBind\tNdx\tName\n";

    for (long i = 0; i < sym_table->symbol_num; i++)
    {
        s_Elf64_Sym* symbol = sym_table->symbols[i];

        string num = to_string(i);
        string value = to_string(symbol->st_value);
        string size = to_string(symbol->st_size);
        string type;
        switch (symbol->type)
        {
            case STT_NOTYPE:
                type = "NOTYP";
                break;
            case STT_SECTION:
                type = "SCTN";
                break;
        }
        string binding;
        switch (symbol->binding)
        {
            case STB_GLOBAL:
                binding = "GLOB";
                break;
            case STB_LOCAL:
                binding = "LOC";
                break;
        }
        string ndx = "UND";
        int section_index = -1;
        if (symbol->section)
            section_index = check_symbol_table(symbol->section->name);
        if (section_index != -1)
            ndx = to_string(section_index);

        string name = symbol->st_name;

        final_string += num + "\t" + value + "\t" + size + "\t" + type + "\t" + binding + "\t" + ndx + "\t" + name + "\n";
    }

    final_string += "\n";

    return final_string;
}

string rela_table_to_string(s_rela_table *rela_table)
{
    string final_string = "#.rela.";
    final_string += rela_table->section->name;
    final_string += "\n";

    final_string += "Offset\tType\tSymbol\tAddend\n";

    for (int i = 0; i < rela_table->entry_num; i++)
    {
        s_Elf64_Rela_entry *entry = rela_table->entries[i];
        if (entry == 0)
            continue;

        string offset = long_to_hex_string(entry->r_offset);
        string type = "NONE";
        switch (entry->reloc_type)
        {
            case R_HIPO_12:
                type = "R_HIPO_12";
                break;
            case R_HIPO_32:
                type = "R_HIPO_32";
                break;
        }
        string symbol = to_string(entry->sym_index);
        string addend = to_string(entry->r_addend);
        final_string += offset + "\t" + type + "\t" + symbol + "\t" + addend + "\n";
    }

    final_string += "\n";

    return final_string;
}

string char_to_hex_string(char x)
{
    const char* hex_digits = "0123456789ABCDEF";

    string hex;
    hex += hex_digits[((unsigned char)x >> 4) & 0xF];
    hex += hex_digits[(unsigned char)x & 0xF];

    return hex;
}


string long_to_hex_string(long x)
{
    const char* hex_digits = "0123456789ABCDEF";

    string hex = "";
    for (int i = 0; i < sizeof(long) * 2; i++)
    {
        hex = hex_digits[(x >> (sizeof(long) * 8 - 4 - i * 4)) & 0xF] + hex;
    }
    return hex;
}

extern "C" char* asm_instruction_name(e_asm_instruction inst)
{
    switch (inst)
    {
        case ASM_INSTR_HALT:
            return string_to_c_string("halt");
        case ASM_INSTR_INT:
            return string_to_c_string("int");
        case ASM_INSTR_IRET:
            return string_to_c_string("iret");
        case ASM_INSTR_CALL:
            return string_to_c_string("call");
        case ASM_INSTR_RET:
            return string_to_c_string("ret");
        case ASM_INSTR_JMP:
            return string_to_c_string("jmp");
        case ASM_INSTR_BEQ:
            return string_to_c_string("beq");
        case ASM_INSTR_BNE:
            return string_to_c_string("bne");
        case ASM_INSTR_BGT:
            return string_to_c_string("bgt");
        case ASM_INSTR_PUSH:
            return string_to_c_string("push");
        case ASM_INSTR_POP:
            return string_to_c_string("pop");
        case ASM_INSTR_XCHG:
            return string_to_c_string("xchg");
        case ASM_INSTR_ADD:
            return string_to_c_string("add");
        case ASM_INSTR_SUB:
            return string_to_c_string("sub");
        case ASM_INSTR_MUL:
            return string_to_c_string("mul");
        case ASM_INSTR_DIV:
            return string_to_c_string("div");
        case ASM_INSTR_NOT:
            return string_to_c_string("not");
        case ASM_INSTR_AND:
            return string_to_c_string("and");
        case ASM_INSTR_OR:
            return string_to_c_string("or");
        case ASM_INSTR_XOR:
            return string_to_c_string("xor");
        case ASM_INSTR_SHL:
            return string_to_c_string("shl");
        case ASM_INSTR_SHR:
            return string_to_c_string("shr");
        case ASM_INSTR_LD:
            return string_to_c_string("ld");
        case ASM_INSTR_ST:
            return string_to_c_string("st");
        case ASM_INSTR_CSRRD:
            return string_to_c_string("csrrd");
        case ASM_INSTR_CSRWR:
            return string_to_c_string("csrwr");
        case ASM_INSTR_NONE:
            return NULL;
    }

    return NULL;
}

extern "C" char* asm_directive_name(e_asm_directive dir)
{
    switch (dir)
    {
        case ASM_DIR_GLOBAL:
            return string_to_c_string("global");
        case ASM_DIR_EXTERN:
            return string_to_c_string("extern");
        case ASM_DIR_SECTION:
            return string_to_c_string("section");
        case ASM_DIR_WORD:
            return string_to_c_string("word");
        case ASM_DIR_SKIP:
            return string_to_c_string("skip");
        case ASM_DIR_ASCII:
            return string_to_c_string("ascii");
        case ASM_DIR_EQU:
            return string_to_c_string("equ");
        case ASM_DIR_END:
            return string_to_c_string("end");
        case ASM_DIR_NONE:
            return NULL;
    }

    return NULL;
}

extern "C" char* asm_line_to_string(s_asm_line *line)
{
    if (line == NULL)
        return string_to_c_string("<null line>\n");

    if (line->is_label)
        return string_to_c_string(nullable_string(line->symbol, "<missing label>") + ":\n");

    if (line->is_directive)
    {
        string result = ".";
        result += owned_c_string_to_string(asm_directive_name(line->directive), "<missing directive>");

        switch (line->directive)
        {
            case ASM_DIR_GLOBAL:
            case ASM_DIR_EXTERN:
                result += " " + symbol_list_to_string(line->symbol_list, line->symbol_list_n);
                break;
            case ASM_DIR_SECTION:
                result += " " + nullable_string(line->section_name, "<missing section>");
                break;
            case ASM_DIR_WORD:
                result += " " + sym_or_lit_list_to_string(line->sym_or_lit_list, line->sym_or_lit_list_n);
                break;
            case ASM_DIR_SKIP:
                result += " " + to_string(line->byte_num);
                break;
            case ASM_DIR_ASCII:
                result += " \"" + nullable_string(line->ascii_string, "<missing string>") + "\"";
                break;
            case ASM_DIR_EQU:
                result += " " + nullable_string(line->new_symbol, "<missing symbol>");
                result += ", " + expression_to_string(line->expression);
                break;
            case ASM_DIR_END:
            case ASM_DIR_NONE:
                break;
        }

        result += "\n";
        return string_to_c_string(result);
    }

    if (line->is_instruction)
    {
        string result = owned_c_string_to_string(asm_instruction_name(line->instruction), "<missing instruction>");

        switch (line->instruction)
        {
            case ASM_INSTR_CALL:
            case ASM_INSTR_JMP:
                result += " " + jump_operand_to_string(line->o_jmp);
                break;
            case ASM_INSTR_BEQ:
            case ASM_INSTR_BNE:
            case ASM_INSTR_BGT:
                result += " " + register_to_string(line->reg1);
                result += ", " + register_to_string(line->reg2);
                result += ", " + jump_operand_to_string(line->o_jmp);
                break;
            case ASM_INSTR_PUSH:
            case ASM_INSTR_POP:
            case ASM_INSTR_NOT:
                result += " " + register_to_string(line->reg1);
                break;
            case ASM_INSTR_XCHG:
            case ASM_INSTR_ADD:
            case ASM_INSTR_SUB:
            case ASM_INSTR_MUL:
            case ASM_INSTR_DIV:
            case ASM_INSTR_AND:
            case ASM_INSTR_OR:
            case ASM_INSTR_XOR:
            case ASM_INSTR_SHL:
            case ASM_INSTR_SHR:
                result += " " + register_to_string(line->reg1);
                result += ", " + register_to_string(line->reg2);
                break;
            case ASM_INSTR_LD:
                result += " " + load_store_operand_to_string(line->o_ls);
                result += ", " + register_to_string(line->reg1);
                break;
            case ASM_INSTR_ST:
                result += " " + register_to_string(line->reg1);
                result += ", " + load_store_operand_to_string(line->o_ls);
                break;
            case ASM_INSTR_CSRRD:
            case ASM_INSTR_CSRWR:
                result += " " + register_to_string(line->reg1);
                result += ", " + register_to_string(line->reg2);
                break;
            case ASM_INSTR_HALT:
            case ASM_INSTR_INT:
            case ASM_INSTR_IRET:
            case ASM_INSTR_RET:
            case ASM_INSTR_NONE:
                break;
        }

        result += "\n";
        return string_to_c_string(result);
    }

    return string_to_c_string("<empty line>\n");
}
