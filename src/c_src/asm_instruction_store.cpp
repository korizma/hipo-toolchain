#include "asm_instruction.hpp"
#include "section.hpp"
#include "asm.hpp"
#include "error.hpp"
#include "machine_instruction.hpp"
#include "misc.hpp"
#include "trampoline.hpp"
#include "symbol_table.hpp"



// literal <= reg1
s_error _handle_st_imm_lit(s_asm_instruction* instruction)
{
    return new_error(ERR_STORE_IMM_VALUE);
}

// symobl <= reg1
s_error _handle_st_imm_sym(s_asm_instruction* instruction)
{
    return new_error(ERR_STORE_IMM_VALUE);
}

// [literal] <= reg1
s_error _handle_st_mem_lit(s_asm_instruction* instruction)
{
    s_section* curr_section = get_current_section();
    s_machine_instruction machine_instr;

    long literal = instruction->load_store_literal;

    if (long_fits_in_12bits(literal))
    {
        machine_instr.operation_code = 0b1000;     // oc for storing data
        machine_instr.modifier = 0b0000;         // modification for mem[A + B + disp] <= C
        machine_instr.reg_a = 0;
        machine_instr.reg_b = 0;
        machine_instr.reg_c = instruction->reg1;
        machine_instr.displacement = literal;         

        write_machine_instr_to_section(curr_section, machine_instr);

        return new_no_error();
    }
    else
    {
        long section_offset = get_section_offset(curr_section);

        machine_instr.operation_code = 0b1000;     // oc for storing data
        machine_instr.modifier = 0b0010;         // modification for mem[mem[A + B + disp]] <= C
        machine_instr.reg_a = ASM_REG_PC;
        machine_instr.reg_b = 0;
        machine_instr.reg_c = instruction->reg1;
        machine_instr.displacement = 0;         // will be changed so it goes to the trampline

        write_machine_instr_to_section(curr_section, machine_instr);

        add_literal_trampoline_entry(curr_section, section_offset+2, section_offset, literal);

        return new_no_error();
    }
}

// [symbol] <= reg1
s_error _handle_st_mem_sym(s_asm_instruction* instruction)
{
    s_section* curr_section = get_current_section();
    s_machine_instruction machine_instr;

    string symbol_name = instruction->load_store_symbol;

    s_symbol_table_entry* symbol = get_and_create_new_symbol_entry(symbol_name);
    long symbol_index = get_symbol_entry_index_by_symbol(symbol_name);

    long section_offset = get_section_offset(curr_section);

    machine_instr.operation_code = 0b1000;     // oc for storing data
    machine_instr.modifier = 0b0010;         // modification for mem[mem[A + B + disp]] <= C
    machine_instr.reg_a = ASM_REG_PC;
    machine_instr.reg_b = 0;
    machine_instr.reg_c = instruction->reg1;
    machine_instr.displacement = 0;         // will be changed so it goes to the trampline

    write_machine_instr_to_section(curr_section, machine_instr);

    add_literal_trampoline_entry(curr_section, section_offset+2, section_offset, symbol_index);

    return new_no_error();
}

// reg2 <= reg1
s_error _handle_st_reg(s_asm_instruction* instruction)
{
    s_section* curr_section = get_current_section();
    s_machine_instruction machine_instr;

    machine_instr.operation_code = 0b1001;     // oc for loading data
    machine_instr.modifier = 0b0001;         // modification A <= B + disp
    machine_instr.reg_a = instruction->load_store_register;
    machine_instr.reg_b = instruction->reg1;
    machine_instr.reg_c = 0;
    machine_instr.displacement = 0;         

    write_machine_instr_to_section(curr_section, machine_instr);

    return new_no_error();
}

// [reg2] <= reg1
s_error _handle_st_reg_mem(s_asm_instruction* instruction)
{
    s_section* curr_section = get_current_section();
    s_machine_instruction machine_instr;

    machine_instr.operation_code = 0b1000;     // oc for storing data
    machine_instr.modifier = 0b0000;         // modification mem[A + B + disp] <= C
    machine_instr.reg_a = instruction->load_store_register;
    machine_instr.reg_b = 0;
    machine_instr.reg_c = instruction->reg1;
    machine_instr.displacement = 0;         

    write_machine_instr_to_section(curr_section, machine_instr);

    return new_no_error();
}

// [reg1 + lit] <= reg2 
s_error _handle_st_reg_mem_lit(s_asm_instruction* instruction)
{
    s_section* curr_section = get_current_section();
    s_machine_instruction machine_instr;

    long literal = instruction->load_store_literal;

    if (long_fits_in_12bits(literal))
    {
        machine_instr.operation_code = 0b1000;     // oc for storing data
        machine_instr.modifier = 0b0000;         // modification for mem[A + B + disp] <= C
        machine_instr.reg_a = instruction->load_store_register;
        machine_instr.reg_b = 0;
        machine_instr.reg_c = instruction->reg1;
        machine_instr.displacement = literal;         

        write_machine_instr_to_section(curr_section, machine_instr);

        return new_no_error();
    }
    else
    {
        return new_error(ERR_LITERAL_TOO_LARGE_LS);
    }
}

// [reg1 + sym] <= reg2 
s_error _handle_st_reg_mem_sym(s_asm_instruction* instruction)
{
    s_section* curr_section = get_current_section();
    s_machine_instruction machine_instr;

    string symbol_name = instruction->load_store_symbol;
    s_symbol_table_entry* symbol = get_and_create_new_symbol_entry(symbol_name);

    long section_offset = get_section_offset(curr_section);

    machine_instr.operation_code = 0b1000;     // oc for storing data
    machine_instr.modifier = 0b0000;         // modification mem[A + B + disp] <= C
    machine_instr.reg_a = instruction->load_store_register;
    machine_instr.reg_b = 0;
    machine_instr.reg_c = instruction->reg1;
    machine_instr.displacement = 0;         

    write_machine_instr_to_section(curr_section, machine_instr);
    add_mem_reg_sym_to_symbol(symbol, curr_section, section_offset, instruction);

    return new_no_error();
}


s_error handle_st(s_asm_instruction* instruction)
{
    switch (instruction->load_store_operand_type)
    {
        case ASM_OP_LS_IMM_LIT:
            return _handle_st_imm_lit(instruction);
            break;
        case ASM_OP_LS_IMM_SYM:
            return _handle_st_imm_sym(instruction);
            break;
        case ASM_OP_LS_MEM_LIT:
            return _handle_st_mem_lit(instruction);
            break;
        case ASM_OP_LS_MEM_SYM:
            return _handle_st_mem_sym(instruction);
            break;
        case ASM_OP_LS_REG:
            return _handle_st_reg(instruction);
            break;
        case ASM_OP_LS_REG_MEM:
            return _handle_st_reg_mem(instruction);
            break;
        case ASM_OP_LS_REG_MEM_LIT:
            return _handle_st_reg_mem_lit(instruction);
            break;
        case ASM_OP_LS_REG_MEM_SYM:
            return _handle_st_reg_mem_sym(instruction);
            break;
    }

    return new_no_error();
}
