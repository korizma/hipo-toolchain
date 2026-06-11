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

int handle_halt(s_asm_line* line, s_section* s)
{
    char oc = find_operation_code(line->instruction);
    char* bin = translate_to_binary(oc, 0, 0, 0, 0, 0);
    write_bytes_to_section(s, bin, INSTRUCTION_BYTE_LEN);
    free(bin);
    return 0;
}

int handle_int(s_asm_line* line, s_section* s)
{
    char oc = find_operation_code(line->instruction);
    char* bin = translate_to_binary(oc, 0, 0, 0, 0, 0);
    write_bytes_to_section(s, bin, INSTRUCTION_BYTE_LEN);
    free(bin);
    return 0;
}

int handle_iret(s_asm_line* line, s_section* s)
{
    // pop status
    // A <= mem32[B], B <= B + D, status <= mem32[sp], sp <= sp + 4
    char oc1 = 0b1001;
    char mod1 = 0b0011;
    char regA1 = ASM_REG_STATUS;
    char regB1 = ASM_REG_SP;
    long disp1 = 4;

    char* bin1 = translate_to_binary(oc1, mod1, regA1, regB1, 0, disp1);
    write_bytes_to_section(s, bin1, INSTRUCTION_BYTE_LEN);


    // pop pc
    // A <= mem32[B], B <= B + D, status <= mem32[sp], sp <= sp + 4
    char oc2 = 0b1001;
    char mod2 = 0b0011;
    char regA2 = ASM_REG_PC;
    char regB2 = ASM_REG_SP;
    long disp2 = 4;

    char* bin2 = translate_to_binary(oc2, mod2, regA2, regB2, 0, disp2);
    write_bytes_to_section(s, bin2, INSTRUCTION_BYTE_LEN);

    free(bin1);
    free(bin2);

    return 0;
}

int handle_call(s_asm_line* line, s_section* s)
{
    char oc = 0b0010, mod, regA, regB;
    long disp;
    if (line->o_jmp.is_symbol)
    {
        int indx = check_symbol_table(line->o_jmp.symbol);
        if (indx == -1)
        {
            // for now error, but will need to add the instruction to a second pass, and save the location of this
            return -1;
        }
        s_Elf64_Sym* sym = p.sym_table->symbols[indx];

        // in the same section, we try to fit in 12b, if not can_fit = false after if
        bool can_fit = false;
        if (sym->section == s)
        {
            long diff = sym->st_value - s->next_free;
            if (diff & 0xFFF == diff)
            {
                disp = diff & 0xFFF;
                can_fit = true;
            }
        }

        if (can_fit)
        {
            mod = 0b0000;
            regA = ASM_REG_PC;
            regB = ASM_REG_R0;
            // disp already set;
        }
        else
        {
            mod = 0b0001;
            // pool settings
        }
    }
    else if (line->o_jmp.is_literal)
    {
        mod = 0b0001;
        // pool settings
    }

    // char* bin = translate_to_binary(oc, mod, regA, regB, 0, disp);
    // write_bytes_to_section(s, bin, INSTRUCTION_BYTE_LEN);

    // free(bin);

    // return 0;
}

int handle_ret(s_asm_line* line, s_section* s)
{
    // pop pc
    // A <= mem32[B], B <= B + D, status <= mem32[sp], sp <= sp + 4
    char op = 0b1001;
    char mod = 0b0011;
    char regA = ASM_REG_PC;
    char regB = ASM_REG_SP;
    long disp = 4;

    char* bin = translate_to_binary(op, mod, regA, regB, 0, disp);
    write_bytes_to_section(s, bin, INSTRUCTION_BYTE_LEN);

    free(bin);

    return 0;
}

int handle_branch(s_asm_line* line, s_section* s)
{
    char oc = 0b0011, mod, regA, regB = line->reg1, regC = line->reg2;

    if (line->instruction == ASM_INSTR_JMP)
    {
        regB = regC = 0;
    }

    long disp;
    if (line->o_jmp.is_symbol)
    {
        int indx = check_symbol_table(line->o_jmp.symbol);
        if (indx == -1)
        {
            // for now error, but will need to add the instruction to a second pass, and save the location of this
            return -1;
        }
        s_Elf64_Sym* sym = p.sym_table->symbols[indx];

        // in the same section, we try to fit in 12b, if not can_fit = false after if
        bool can_fit = false;
        if (sym->section == s)
        {
            long diff = sym->st_value - s->next_free;
            if (diff & 0xFFF == diff)
            {
                disp = diff & 0xFFF;
                can_fit = true;
            }
        }

        if (can_fit)
        {
            switch(line->instruction)
            {
                case ASM_INSTR_BEQ: mod = 0b0001; break;
                case ASM_INSTR_BGT: mod = 0b0011; break;
                case ASM_INSTR_BNE: mod = 0b0010; break;
                case ASM_INSTR_JMP: mod = 0b0000; break;
            }
            regA = ASM_REG_PC;
            // disp already set;
        }
        else
        {
            switch(line->instruction)
            {
                case ASM_INSTR_BEQ: mod = 0b1001; break;
                case ASM_INSTR_BGT: mod = 0b1011; break;
                case ASM_INSTR_BNE: mod = 0b1010; break;
                case ASM_INSTR_JMP: mod = 0b1000; break;
            }
            // pool settings
        }
    }
    else if (line->o_jmp.is_literal)
    {
        switch(line->instruction)
            {
                case ASM_INSTR_BEQ: mod = 0b1001; break;
                case ASM_INSTR_BGT: mod = 0b1011; break;
                case ASM_INSTR_BNE: mod = 0b1010; break;
                case ASM_INSTR_JMP: mod = 0b1000; break;
            }
        // pool settings
    }

    // char* bin = translate_to_binary(oc, mod, regA, regB, 0, disp);
    // write_bytes_to_section(s, bin, INSTRUCTION_BYTE_LEN);

    // free(bin);

    // return 0;
}

int handle_push(s_asm_line* line, s_section* s)
{
    // write to sp - 4, because sp points to the top of the stack that is occupied
    // push reg <=> ld reg, [sp - 4]; sp = sp - 4;
    // instead of one instruction, we need 2 operations
    // first mem32[A + B + D] <= C, mem[sp - 4] <= reg
    char op1 = 0b1000;
    char mod1 = 0b0000;
    char regA1 = ASM_REG_SP;
    char regB1 = ASM_REG_R0;
    char regC1 = line->reg1;
    long disp1 = -4;

    char* bin1 = translate_to_binary(op1, mod1, regA1, regB1, regC1, disp1);
    write_bytes_to_section(s, bin1, INSTRUCTION_BYTE_LEN);
    
    // second A <= C + D, sp <= sp - 4
    char op2 = 0b1001;
    char mod2 = 0b0001;
    char regA2 = ASM_REG_SP;
    char regB2 = ASM_REG_SP;
    long disp2 = -4;

    char* bin2 = translate_to_binary(op2, mod2, regA2, regB2, 0, disp2);
    write_bytes_to_section(s, bin2, INSTRUCTION_BYTE_LEN);

    free(bin1);
    free(bin2);

    return 0;
}

int handle_pop(s_asm_line* line, s_section* s)
{
    // reg <= mem[sp]; sp <= sp + 4;
    // A <= mem32[B], B <= B + D, reg <= mem32[sp], sp <= sp + 4
    char op = 0b1001;
    char mod = 0b0011;
    char regA = line->reg1;
    char regB = ASM_REG_SP;
    long disp = 4;
    char* bin = translate_to_binary(op, mod, regA, regB, 0, disp);
    write_bytes_to_section(s, bin, INSTRUCTION_BYTE_LEN);
    free(bin);
}

int handle_xchg(s_asm_line* line, s_section* s)
{
    char oc = find_operation_code(line->instruction);

    char* bin = translate_to_binary(oc, 0, 0, line->reg1, line->reg2, 0);
    write_bytes_to_section(s, bin, INSTRUCTION_BYTE_LEN);

    free(bin);

    return 0;
}

int handle_arthm(s_asm_line* line, s_section* s)
{
    char oc = find_operation_code(line->instruction);
    char mod = find_operation_mod(line);

    char* bin = translate_to_binary(oc, mod, line->reg1, line->reg1, line->reg2, 0);
    write_bytes_to_section(s, bin, INSTRUCTION_BYTE_LEN);

    free(bin);

    return 0;
}

int handle_not(s_asm_line* line, s_section* s)
{
    char oc = find_operation_code(line->instruction);
    char mod = find_operation_mod(line);

    char* bin = translate_to_binary(oc, mod, line->reg1, line->reg1, 0, 0);
    write_bytes_to_section(s, bin, INSTRUCTION_BYTE_LEN);

    free(bin);

    return 0;
}

int handle_logic(s_asm_line* line, s_section* s)
{
    char oc = find_operation_code(line->instruction);
    char mod = find_operation_mod(line);

    char* bin = translate_to_binary(oc, mod, line->reg1, line->reg1, line->reg2, 0);
    write_bytes_to_section(s, bin, INSTRUCTION_BYTE_LEN);

    free(bin);

    return 0;
}

int handle_sh(s_asm_line* line, s_section* s)
{
    char oc = find_operation_code(line->instruction);
    char mod = find_operation_mod(line);

    char* bin = translate_to_binary(oc, mod, line->reg1, line->reg1, line->reg2, 0);
    write_bytes_to_section(s, bin, INSTRUCTION_BYTE_LEN);

    free(bin);

    return 0;
}

// ld operand, reg1 <=> reg1 <= operand
int handle_ld(s_asm_line* line, s_section* s)
{
    char oc = 0b1001, mod, regA = 0, regB = 0, regC = 0;
    long disp = 0;

    if (line->o_ls.kind == ASM_OPERAND_LS_IMM_LITERAL)
    {
        // A <= B + D
        mod = 0b0001;
        regA = line->reg1;
        regB = ASM_REG_R0;

        if (line->o_ls.literal != line->o_ls.literal & 0xFFF)
        {
            printf("ERROR: ld operand: IMM LITERAL %l is bigger than 12 bits!\n", line->o_ls.literal);
            return -1;
        }

        disp = line->o_ls.literal & 0xFFF;
    }
    else if (line->o_ls.kind == ASM_OPERAND_LS_IMM_SYMBOL)
    {
        // A <= B + D
        mod = 0b0001;
        regA = line->reg1;
        regB = ASM_REG_R0;
        // create relocation for the disp
    }
    else if (line->o_ls.kind == ASM_OPERAND_LS_MEM_LITERAL)
    {
        // A <= mem32[B + C + D]
        mod = 0b0010;
        regA = line->reg1;
        regB = regC = ASM_REG_R0;

        if (line->o_ls.literal != line->o_ls.literal & 0xFFF)
        {
            printf("ERROR: ld operand: MEM LITERAL %l is bigger than 12 bits!\n", line->o_ls.literal);
            return -1;
        }

        disp = line->o_ls.literal & 0xFFF; 
    }
    else if (line->o_ls.kind == ASM_OPERAND_LS_MEM_SYMBOL)
    {
        // A <= mem32[B + C + D]
        mod = 0b0010;
        regA = line->reg1;
        regB = regC = ASM_REG_R0;
        // create relocation for disp
    }
    else if (line->o_ls.kind == ASM_OPERAND_LS_REG)
    {
        // A <= B + D
        mod = 0b0001;
        regA = line->reg1;
        regB = line->o_ls.reg;
        disp = 0;
    }
    else if (line->o_ls.kind == ASM_OPERAND_LS_REG_INDIRECT)
    {
        // A <= mem32[B + C + D]
        mod = 0b0010;
        regA = line->reg1;
        regB = line->o_ls.reg;
        regC = 0;
        disp = 0;
    }
    else if (line->o_ls.kind == ASM_OPERAND_LS_REG_INDIRECT_LITERAL)
    {
        // A <= mem32[B + C + D]
        mod = 0b0010;
        regA = line->reg1;
        regB = line->o_ls.reg;
        regC = 0;

        if (line->o_ls.literal != line->o_ls.literal & 0xFFF)
        {
            printf("ERROR: ld operand: REG INDIRECT LITERAL %l is bigger than 12 bits!\n", line->o_ls.literal);
            return -1;
        }

        disp = line->o_ls.literal & 0xFFF; 
    }
    else if (line->o_ls.kind == ASM_OPERAND_LS_REG_INDIRECT_LITERAL)
    {
        // A <= mem32[B + C + D]
        mod = 0b0010;
        regA = line->reg1;
        regB = line->o_ls.reg;
        regC = 0;
        // create relocation for disp
    }

    char* bin = translate_to_binary(oc, mod, regA, regB, regC, disp);
    write_bytes_to_section(s, bin, INSTRUCTION_BYTE_LEN);

    free(bin);

    return 0;
}

// st reg, operand; operand <= return 
int handle_st(s_asm_line* line, s_section* s)
{
    char oc = 0b1000, mod, regA = 0, regB = 0, regC = 0;
    long disp = 0;

    if (line->o_ls.kind == ASM_OPERAND_LS_IMM_LITERAL)
    {
        printf("ERROR: st operand: Invalid operand: IMM_LITERAL!");
        return -1;
    }
    else if (line->o_ls.kind == ASM_OPERAND_LS_IMM_SYMBOL)
    {
        printf("ERROR: st operand: Invalid operand: IMM_SYMBOL!");
        return -1;
    }
    else if (line->o_ls.kind == ASM_OPERAND_LS_MEM_LITERAL)
    {
        // mem[A + B + D] <= C
        mod = 0b0000;
        regA = regB = 0;
        regC = line->reg1;
        
        if (line->o_ls.literal != line->o_ls.literal & 0xFFF)
        {
            printf("ERROR: st operand: REG INDIRECT LITERAL %l is bigger than 12 bits!\n", line->o_ls.literal);
            return -1;
        }

        disp = line->o_ls.literal & 0xFFF; 
    }
    else if (line->o_ls.kind == ASM_OPERAND_LS_MEM_SYMBOL)
    {
        // mem[A + B + D] <= C
        mod = 0b0000;
        regA = regB = 0;
        regC = line->reg1;

        // create relocation for disp
    }
    else if (line->o_ls.kind == ASM_OPERAND_LS_REG)
    {
        // A <= B + D
        oc =  0b1001;
        mod = 0b0001;
        regA = line->o_ls.reg;
        regB = line->reg1;
        disp = 0;
    }
    else if (line->o_ls.kind == ASM_OPERAND_LS_REG_INDIRECT)
    {
        // mem[A + B + D] <= C
        mod = 0b0000;
        regA = line->o_ls.reg; 
        regB = 0;
        regC = line->reg1;
        disp = 0;
    }
    else if (line->o_ls.kind == ASM_OPERAND_LS_REG_INDIRECT_LITERAL)
    {
        // mem[A + B + D] <= C
        mod = 0b0000;
        regA = line->o_ls.reg; 
        regB = 0;
        regC = line->reg1;
        
        if (line->o_ls.literal != line->o_ls.literal & 0xFFF)
        {
            printf("ERROR: st operand: REG INDIRECT LITERAL %l is bigger than 12 bits!\n", line->o_ls.literal);
            return -1;
        }

        disp = line->o_ls.literal & 0xFFF; 
    }
    else if (line->o_ls.kind == ASM_OPERAND_LS_REG_INDIRECT_LITERAL)
    {
        // mem[A + B + D] <= C
        mod = 0b0000;
        regA = line->o_ls.reg; 
        regB = 0;
        regC = line->reg1;

        // create relocation for disp
    }

    char* bin = translate_to_binary(oc, mod, regA, regB, regC, disp);
    write_bytes_to_section(s, bin, INSTRUCTION_BYTE_LEN);

    free(bin);

    return 0;
}

// csrrd %csr, %gpr; gpr <= csr   
// reg1 = csr, reg2 = gpr    
// csrwr %gpr, %csr; csr <= gpr;  
// reg1 = gpr, reg2 = csr
int handle_control_rw(s_asm_line* line, s_section* s)
{
    // A<=B
    char oc = find_operation_code(line->instruction);
    char mod = find_operation_mod(line);
    char regA = line->reg2, regB = line->reg1;
    char* bin = translate_to_binary(oc, mod, regA, regB, 0, 0);
    write_bytes_to_section(s, bin, INSTRUCTION_BYTE_LEN);

    free(bin);

    return 0;
}


int handle_global(s_asm_line* line, s_section* s)
{

}
int handle_extern(s_asm_line* line, s_section* s)
{
    char type = STT_NOTYPE;
    add_to_symbol_table(line->symbol, type, STB_GLOBAL, STV_DEFAULT, 0, 0, 0);
}
int handle_section(s_asm_line* line, s_section* s)
{

}
int handle_word(s_asm_line* line, s_section* s)
{

}
int handle_skip(s_asm_line* line, s_section* s)
{

}
int handle_ascii(s_asm_line* line, s_section* s)
{

}
int handle_equ(s_asm_line* line, s_section* s)
{

}
int handle_end(s_asm_line* line, s_section* s)
{
    add_to_symbol_table(s->name, STT_SECTION, STB_LOCAL, STV_DEFAULT, s, s->next_free, 0);
    add_section_to_program(s);
    p.curr_section = 0;
}

int handle_label(s_asm_line* line, s_section* s)
{
    char type = STT_NOTYPE;
    if (strcmp(s->name, "common") == 0)
        type = STT_COMMON;
    add_to_symbol_table(line->symbol, type, STB_LOCAL, STV_DEFAULT, s, s->next_free, 0);
    // sym size needs to be set only if the next directive is ascii, word or skip
}

char* translate_to_binary(char oc, char mod, char reg_a, char reg_b, char reg_c, long disp)
{
    char* bin = (char*)malloc(sizeof(char)*INSTRUCTION_BYTE_LEN);
    // OC MOD - byte 3
    bin[3] = mod | (oc << 4);
    // regA regB - byte 2
    bin[1] = reg_b | (reg_a << 4);
    // regC DISP - byte 1
    bin[3] = ((disp >> 8) & 0b11111111) | (reg_c << 4);
    // DISP DISP - byte 0
    bin[0] = disp & 0b11111111;

    return bin;
}

// just placeholder
bool expr_is_valid(s_expr* expression, s_section* curr_section)
{
    return true;
}

// just placeholder
long calculate_expression(s_expr* expression)
{
    return 0;
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
        if (operation_to_oc[i][0] == line->instruction) {
            return operation_to_oc[i][1];
        }
    }
    return -1; // not found
}
