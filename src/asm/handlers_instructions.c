#include "handlers_impl.h"
#include "code.h"
#include "trampoline.h"
#include <stdlib.h>
#include <string.h>

extern s_program p;

s_error* handle_halt(s_asm_line* line, s_section* s)
{
    if (s == 0)
    {
        return new_error(line, ERR_OUTSIDE_SECTION);
    }

    char oc = find_operation_code(line->instruction);
    char* bin = translate_to_binary(oc, 0, 0, 0, 0, 0);
    write_bytes_to_section(s, bin, INSTRUCTION_BYTE_LEN);
    free(bin);
    return NULL;
}

s_error* handle_int(s_asm_line* line, s_section* s)
{
    if (s == 0)
    {
        return new_error(line, ERR_OUTSIDE_SECTION);
    }

    char oc = find_operation_code(line->instruction);
    char* bin = translate_to_binary(oc, 0, 0, 0, 0, 0);
    write_bytes_to_section(s, bin, INSTRUCTION_BYTE_LEN);
    free(bin);
    return NULL;
}

s_error* handle_iret(s_asm_line* line, s_section* s)
{
    if (s == 0)
    {
        return new_error(line, ERR_OUTSIDE_SECTION);
    }

    // pop status
    // A <= mem32[B], B <= B + D, status <= mem32[sp], sp <= sp + 4
    char oc1 = 0b1001;
    char mod1 = 0b0111;
    char regA1 = ASM_REG_STATUS % 16;
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

    return NULL;
}

s_error* handle_call(s_asm_line* line, s_section* s)
{
    if (s == 0)
    {
        return new_error(line, ERR_OUTSIDE_SECTION);
    }

    char oc = 0b0010, mod, regA, regB;
    long disp;
    if (line->o_jmp.is_symbol)
    {
        int indx = get_and_set_reference(line->o_jmp.symbol);

        s_Elf64_Sym* sym = p.sym_table->symbols[indx];
        if (sym->type == STT_SECTION)
        {
            return new_symbol_error(line, ERR_SECTION_AS_DESTINATION, sym->st_name);
        }

        // trampoline pool
        add_trampoline_entry(s, line, 0, sym->st_name, TE_JUMP_SYMBOL);
        
        mod = 0b0001;
        regA = ASM_REG_PC;
        regB = ASM_REG_R0;

        line->section_location = s;
        line->bytes_location = s->next_free;

        char* bin = translate_to_binary(oc, mod, regA, regB, 0, 0);
        write_bytes_to_section(s, bin, INSTRUCTION_BYTE_LEN);

        return NULL;
    }
    else if (line->o_jmp.is_literal)
    {
        if (long_fit_in_12b(line->o_jmp.literal))
        {
            // literal small enough to fit in 12b 
            mod = 0b0000;
            regA = ASM_REG_PC;
            regB = ASM_REG_R0;
            disp = line->o_jmp.literal & 0xFFF;

            char* bin = translate_to_binary(oc, mod, regA, regB, 0, disp);
            write_bytes_to_section(s, bin, INSTRUCTION_BYTE_LEN);

            return NULL;
        }

        add_trampoline_entry(s, line, line->o_jmp.literal, 0, TE_JUMP_LITERAL);
        
        mod = 0b0001;
        regA = ASM_REG_PC;
        regB = ASM_REG_R0;

        line->section_location = s;
        line->bytes_location = s->next_free;

        char* bin = translate_to_binary(oc, mod, regA, regB, 0, 0);
        write_bytes_to_section(s, bin, INSTRUCTION_BYTE_LEN);

        return NULL;
    }
    return new_error(line, ERR_INVALID_JUMP_OPERAND);
}

s_error* handle_ret(s_asm_line* line, s_section* s)
{
    if (s == 0)
    {
        return new_error(line, ERR_OUTSIDE_SECTION);
    }

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

    return NULL;
}

s_error* handle_branch(s_asm_line* line, s_section* s)
{
    if (s == 0)
    {
        return new_error(line, ERR_OUTSIDE_SECTION);
    }

    char oc = 0b0011, mod, regA, regB = line->reg1, regC = line->reg2;

    if (line->instruction == ASM_INSTR_JMP)
    {
        regB = regC = 0;
    }

    long disp;
    if (line->o_jmp.is_symbol)
    {
        int indx = get_and_set_reference(line->o_jmp.symbol);

        s_Elf64_Sym* sym = p.sym_table->symbols[indx];

        if (sym->type == STT_SECTION)
        {
            return new_symbol_error(line, ERR_SECTION_AS_DESTINATION, sym->st_name);
        }
        
        // trampoline pool
        add_trampoline_entry(s, line, 0, sym->st_name, TE_JUMP_SYMBOL);
        
        switch(line->instruction)
        {
            case ASM_INSTR_BEQ: mod = 0b1001; break;
            case ASM_INSTR_BGT: mod = 0b1011; break;
            case ASM_INSTR_BNE: mod = 0b1010; break;
            case ASM_INSTR_JMP: mod = 0b1000; break;
        }
        regA = ASM_REG_PC;

        line->section_location = s;
        line->bytes_location = s->next_free;

        char* bin = translate_to_binary(oc, mod, regA, regB, regC, 0);
        write_bytes_to_section(s, bin, INSTRUCTION_BYTE_LEN);

        return NULL;       
    }
    else if (line->o_jmp.is_literal)
    {
        
        // pool settings

        if (long_fit_in_12b(line->o_jmp.literal))
        {
            // literal small enough to fit in 12b 
            switch(line->instruction)
            {
                case ASM_INSTR_BEQ: mod = 0b1001; break;
                case ASM_INSTR_BGT: mod = 0b1011; break;
                case ASM_INSTR_BNE: mod = 0b1010; break;
                case ASM_INSTR_JMP: mod = 0b1000; break;
            }
            regA = ASM_REG_R0;
            disp = line->o_jmp.literal & 0xFFF;

            char* bin = translate_to_binary(oc, mod, regA, regB, 0, disp);
            write_bytes_to_section(s, bin, INSTRUCTION_BYTE_LEN);

            return NULL;
        }

        add_trampoline_entry(s, line, line->o_jmp.literal, 0, TE_JUMP_LITERAL);
        
        switch(line->instruction)
        {
            case ASM_INSTR_BEQ: mod = 0b1001; break;
            case ASM_INSTR_BGT: mod = 0b1011; break;
            case ASM_INSTR_BNE: mod = 0b1010; break;
            case ASM_INSTR_JMP: mod = 0b1000; break;
        }
        regA = ASM_REG_PC;

        line->section_location = s;
        line->bytes_location = s->next_free;

        char* bin = translate_to_binary(oc, mod, regA, regB, 0, 0);
        write_bytes_to_section(s, bin, INSTRUCTION_BYTE_LEN);

        return NULL;
    }
    return new_error(line, ERR_INVALID_JUMP_OPERAND);
}

s_error* handle_push(s_asm_line* line, s_section* s)
{
    if (s == 0)
    {
        return new_error(line, ERR_OUTSIDE_SECTION);
    }
    // OC=0x8 MOD=0x1 A=sp B=0 C=gpr D=-4
    char op = 0b1000;
    char mod = 0b0001;
    char regA = ASM_REG_SP;
    char regB= ASM_REG_R0;
    char regC = line->reg1;
    long disp = -4;

    char* bin = translate_to_binary(op, mod, regA, regB, regC, disp);
    write_bytes_to_section(s, bin, INSTRUCTION_BYTE_LEN);

    free(bin);

    return NULL;
}

s_error* handle_pop(s_asm_line* line, s_section* s)
{
    if (s == 0)
    {
        return new_error(line, ERR_OUTSIDE_SECTION);
    }

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

    return NULL;
}

s_error* handle_xchg(s_asm_line* line, s_section* s)
{
    if (s == 0)
    {
        return new_error(line, ERR_OUTSIDE_SECTION);
    }

    char oc = find_operation_code(line->instruction);

    char* bin = translate_to_binary(oc, 0, 0, line->reg1, line->reg2, 0);
    write_bytes_to_section(s, bin, INSTRUCTION_BYTE_LEN);

    free(bin);

    return NULL;
}

s_error* handle_arthm(s_asm_line* line, s_section* s)
{
    if (s == 0)
    {
        return new_error(line, ERR_OUTSIDE_SECTION);
    }

    char oc = find_operation_code(line->instruction);
    char mod = find_operation_mod(line);

    char* bin = translate_to_binary(oc, mod, line->reg2, line->reg2, line->reg1, 0);
    write_bytes_to_section(s, bin, INSTRUCTION_BYTE_LEN);

    free(bin);

    return NULL;
}

s_error* handle_not(s_asm_line* line, s_section* s)
{
    if (s == 0)
    {
        return new_error(line, ERR_OUTSIDE_SECTION);
    }

    char oc = find_operation_code(line->instruction);
    char mod = find_operation_mod(line);

    char* bin = translate_to_binary(oc, mod, line->reg1, line->reg1, 0, 0);
    write_bytes_to_section(s, bin, INSTRUCTION_BYTE_LEN);

    free(bin);

    return NULL;
}

s_error* handle_logic(s_asm_line* line, s_section* s)
{
    if (s == 0)
    {
        return new_error(line, ERR_OUTSIDE_SECTION);
    }

    char oc = find_operation_code(line->instruction);
    char mod = find_operation_mod(line);

    char* bin = translate_to_binary(oc, mod, line->reg2, line->reg2, line->reg1, 0);
    write_bytes_to_section(s, bin, INSTRUCTION_BYTE_LEN);

    free(bin);

    return NULL;
}

s_error* handle_sh(s_asm_line* line, s_section* s)
{
    if (s == 0)
    {
        return new_error(line, ERR_OUTSIDE_SECTION);
    }

    char oc = find_operation_code(line->instruction);
    char mod = find_operation_mod(line);

    char* bin = translate_to_binary(oc, mod, line->reg2, line->reg2, line->reg1, 0);
    write_bytes_to_section(s, bin, INSTRUCTION_BYTE_LEN);

    free(bin);

    return NULL;
}

// ld operand, reg1 <=> reg1 <= operand
s_error* handle_ld(s_asm_line* line, s_section* s)
{
    if (s == 0)
    {
        return new_error(line, ERR_OUTSIDE_SECTION);
    }

    char oc = 0b1001, mod, regA = 0, regB = 0, regC = 0;
    long disp = 0;

    if (line->o_ls.kind == ASM_OPERAND_LS_IMM_LITERAL)
    {
        // A <= B + D
        mod = 0b0001;
        regA = line->reg1;
        regB = ASM_REG_R0;

        if (!long_fit_in_12b(line->o_ls.literal))
        {
            // the literal does not fit in 12bits so we need to use it from the pool
            // using OC=1001, MOD = 0010
            // A <= mem[B + C + D]
            add_trampoline_entry(s, line, line->o_ls.literal, 0, TE_LD_IMM_LITERAL);

            oc = 0b1001;
            mod = 0b0010;
            regA = line->reg1;
            regB = ASM_REG_PC;
            regC = ASM_REG_R0;

            line->section_location = s;
            line->bytes_location = s->next_free;

            // disp will be edited in trampoline
            char* bin = translate_to_binary(oc, mod, regA, regB, regC, 0);
            write_bytes_to_section(s, bin, INSTRUCTION_BYTE_LEN);

            free(bin);
            return NULL;
        }

        disp = line->o_ls.literal & 0xFFF;
    }
    else if (line->o_ls.kind == ASM_OPERAND_LS_IMM_SYMBOL)
    {
        int indx = get_and_set_reference(line->o_ls.symbol);

        s_Elf64_Sym* sym = p.sym_table->symbols[indx];

        // the symbol does not fit in 12bits so we need to use it from the pool
        // using OC=1001, MOD = 0010
        // A <= mem[B + C + D]
        add_trampoline_entry(s, line, 0, sym->st_name, TE_LD_IMM_SYMBOL);

        oc = 0b1001;
        mod = 0b0010;
        regA = line->reg1;
        regB = ASM_REG_PC;
        regC = ASM_REG_R0;

        line->section_location = s;
        line->bytes_location = s->next_free;

        // disp will be edited in trampoline
        // and trampoline code will emit the rela
        char* bin = translate_to_binary(oc, mod, regA, regB, regC, 0);
        write_bytes_to_section(s, bin, INSTRUCTION_BYTE_LEN);

        free(bin);
        return NULL;
    }
    else if (line->o_ls.kind == ASM_OPERAND_LS_MEM_LITERAL)
    {
        

        if (long_fit_in_12b(line->o_ls.literal))
        {
            // the literal does not fit in 12bits so we need to use it from the pool
            // using OC=1001, MOD = 0010
            // A <= mem[B + C + D]
            add_trampoline_entry(s, line, line->o_ls.literal, 0, TE_LD_IMM_LITERAL);

            oc = 0b1001;
            mod = 0b0010;
            regA = line->reg1;
            regB = ASM_REG_PC;
            regC = ASM_REG_R0;

            line->section_location = s;
            line->bytes_location = s->next_free;

            // disp will be edited in trampoline
            char* bin = translate_to_binary(oc, mod, regA, regB, regC, 0);
            write_bytes_to_section(s, bin, INSTRUCTION_BYTE_LEN);

            free(bin);
        }
        else
        {   
            // reg <= symbol
            // A <= B + D
            oc = 0b1001;
            mod = 0b0001;
            regA = line->reg1;
            regB = ASM_REG_R0;
            disp = line->o_ls.literal & 0xFFF;

            line->section_location = s;
            line->bytes_location = s->next_free;

            char* bin = translate_to_binary(oc, mod, regA, regB, regC, 0);
            write_bytes_to_section(s, bin, INSTRUCTION_BYTE_LEN);

            free(bin);
        }


        // A <= mem32[B + C + D]
        // reg <= mem32[literal]
        mod = 0b0010;
        regA = line->reg1;
        regB = line->reg1;
        regC = 0;
        disp = 0;
    }
    else if (line->o_ls.kind == ASM_OPERAND_LS_MEM_SYMBOL)
    {
        

        if (long_fit_in_12b(line->o_ls.literal))
        {
            // the literal does not fit in 12bits so we need to use it from the pool
            // using OC=1001, MOD = 0010
            // A <= mem[B + C + D]
            add_trampoline_entry(s, line, 0, line->o_ls.symbol, TE_LD_IMM_SYMBOL);

            oc = 0b1001;
            mod = 0b0010;
            regA = line->reg1;
            regB = ASM_REG_PC;
            regC = ASM_REG_R0;

            line->section_location = s;
            line->bytes_location = s->next_free;

            // disp will be edited in trampoline
            char* bin = translate_to_binary(oc, mod, regA, regB, regC, 0);
            write_bytes_to_section(s, bin, INSTRUCTION_BYTE_LEN);

            free(bin);
        }
        else
        {   
           // reg <= symbol
            // A <= B + D
            oc = 0b1001;
            mod = 0b0001;
            regA = line->reg1;
            regB = ASM_REG_R0;
            disp = line->o_ls.literal & 0xFFF;

            line->section_location = s;
            line->bytes_location = s->next_free;

            char* bin = translate_to_binary(oc, mod, regA, regB, regC, 0);
            write_bytes_to_section(s, bin, INSTRUCTION_BYTE_LEN);

            free(bin);
        }


        // A <= mem32[B + C + D]
        // reg <= mem32[literal]
        mod = 0b0010;
        regA = line->reg1;
        regB = line->reg1;
        regC = 0;
        disp = 0;
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

        if (!long_fit_in_12b(line->o_ls.literal))
        {
            return new_operand_literal_error(line, ERR_LITERAL_OUT_OF_RANGE_12B, line->o_ls.kind, line->o_ls.literal);
        }

        disp = line->o_ls.literal & 0xFFF; 
    }
    else if (line->o_ls.kind == ASM_OPERAND_LS_REG_INDIRECT_SYMBOL)
    {
        // symbol is always > 12bits, unless its equ and has deterministic value
        // equ not implemented
        // throwing error for now
        return new_operand_error(line, ERR_UNSUPPORTED_OPERAND, line->o_ls.kind);
    }

    char* bin = translate_to_binary(oc, mod, regA, regB, regC, disp);
    write_bytes_to_section(s, bin, INSTRUCTION_BYTE_LEN);

    free(bin);

    return NULL;
}

// st reg, operand; operand <= return 
s_error* handle_st(s_asm_line* line, s_section* s)
{
    if (s == 0)
    {
        return new_error(line, ERR_OUTSIDE_SECTION);
    }

    char oc = 0b1000, mod, regA = 0, regB = 0, regC = 0;
    long disp = 0;

    if (line->o_ls.kind == ASM_OPERAND_LS_IMM_LITERAL)
    {
        return new_operand_error(line, ERR_INVALID_OPERAND, line->o_ls.kind);
    }
    else if (line->o_ls.kind == ASM_OPERAND_LS_IMM_SYMBOL)
    {
        return new_operand_error(line, ERR_INVALID_OPERAND, line->o_ls.kind);
    }
    else if (line->o_ls.kind == ASM_OPERAND_LS_MEM_LITERAL)
    {
        // mem[A + B + D] <= C
        mod = 0b0000;
        regA = regB = 0;
        regC = line->reg1;
        
        if (!long_fit_in_12b(line->o_ls.literal))
        {
            // oc = 0b1000, mod=0b0010
            // mem[mem[A + B + D]] <= C
            add_trampoline_entry(s, line, line->o_ls.literal, 0, TE_ST_MEM_LITERAL);

            oc = 0b1000;
            mod = 0b0010;
            regA = ASM_REG_PC;
            regB = ASM_REG_R0;
            regC = line->reg1;

            line->section_location = s;
            line->bytes_location = s->next_free;

            // disp will be edited in trampoline
            char* bin = translate_to_binary(oc, mod, regA, regB, regC, 0);
            write_bytes_to_section(s, bin, INSTRUCTION_BYTE_LEN);

            free(bin);
            return NULL;
        }

        disp = line->o_ls.literal & 0xFFF; 
    }
    else if (line->o_ls.kind == ASM_OPERAND_LS_MEM_SYMBOL)
    {
        // create relocation for disp
        int indx = get_and_set_reference(line->o_ls.symbol);

        s_Elf64_Sym* sym = p.sym_table->symbols[indx];

        // oc = 0b1000, mod=0b0010
        // mem[mem[A + B + D]] <= C
        add_trampoline_entry(s, line, 0, sym->st_name, TE_ST_MEM_SYMBOL);

        oc = 0b1000;
        mod = 0b0010;
        regA = ASM_REG_PC;
        regB = ASM_REG_R0;
        regC = line->reg1;

        line->section_location = s;
        line->bytes_location = s->next_free;

        // disp will be edited in trampoline
        char* bin = translate_to_binary(oc, mod, regA, regB, regC, 0);
        write_bytes_to_section(s, bin, INSTRUCTION_BYTE_LEN);

        free(bin);
        return NULL;
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
        
        if (!long_fit_in_12b(line->o_ls.literal))
        {
            return new_operand_literal_error(line, ERR_LITERAL_OUT_OF_RANGE_12B, line->o_ls.kind, line->o_ls.literal);
        }

        disp = line->o_ls.literal & 0xFFF; 
    }
    else if (line->o_ls.kind == ASM_OPERAND_LS_REG_INDIRECT_SYMBOL)
    {
        // symbol is always > 12bits, unless its equ and has deterministic value
        // equ not implemented
        // throwing error for now
        return new_operand_error(line, ERR_INVALID_OPERAND, line->o_ls.kind);
    }

    char* bin = translate_to_binary(oc, mod, regA, regB, regC, disp);
    write_bytes_to_section(s, bin, INSTRUCTION_BYTE_LEN);

    free(bin);

    return NULL;
}

// csrrd %csr, %gpr; gpr <= csr   
// reg1 = csr, reg2 = gpr    
// csrwr %gpr, %csr; csr <= gpr;  
// reg1 = gpr, reg2 = csr
s_error* handle_control_rw(s_asm_line* line, s_section* s)
{
    if (s == 0)
    {
        return new_error(line, ERR_OUTSIDE_SECTION);
    }

    // A<=B
    char oc = find_operation_code(line->instruction);
    char mod = find_operation_mod(line);
    char regA = line->reg2 % 16, regB = line->reg1 % 16;
    char* bin = translate_to_binary(oc, mod, regA, regB, 0, 0);
    write_bytes_to_section(s, bin, INSTRUCTION_BYTE_LEN);

    free(bin);

    return NULL;
}
