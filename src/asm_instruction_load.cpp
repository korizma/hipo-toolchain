#include "asm_instruction.hpp"
#include "section.hpp"
#include "asm.hpp"
#include "error.hpp"
#include "machine_instruction.hpp"
#include "misc.hpp"
#include "trampoline.hpp"
#include "symbol_table.hpp"

s_error _handle_ld_imm_lit(s_program* program, s_asm_instruction* instruction);
s_error _handle_ld_imm_sym(s_program* program, s_asm_instruction* instruction);
s_error _handle_ld_mem_lit(s_program* program, s_asm_instruction* instruction);
s_error _handle_ld_mem_sym(s_program* program, s_asm_instruction* instruction);
s_error _handle_ld_reg(s_program* program, s_asm_instruction* instruction);
s_error _handle_ld_reg_mem(s_program* program, s_asm_instruction* instruction);
s_error _handle_ld_reg_mem_lit(s_program* program, s_asm_instruction* instruction);
s_error _handle_ld_reg_mem_sym(s_program* program, s_asm_instruction* instruction);


s_error handle_ld(s_program* program, s_asm_instruction* instruction)
{
    switch (instruction->load_store_operand_type)
    {
        case ASM_OP_LS_IMM_LIT:
            return _handle_ld_imm_lit(program, instruction);
        case ASM_OP_LS_IMM_SYM:
            return _handle_ld_imm_sym(program, instruction);
        case ASM_OP_LS_MEM_LIT:
            return _handle_ld_mem_lit(program, instruction);
        case ASM_OP_LS_MEM_SYM:
            return _handle_ld_mem_sym(program, instruction);
        case ASM_OP_LS_REG:
            return _handle_ld_reg(program, instruction);
        case ASM_OP_LS_REG_MEM:
            return _handle_ld_reg_mem(program, instruction);
        case ASM_OP_LS_REG_MEM_LIT:
            return _handle_ld_reg_mem_lit(program, instruction);
        case ASM_OP_LS_REG_MEM_SYM:
            return _handle_ld_reg_mem_sym(program, instruction);
    }

    return new_no_error();
}

// reg1 <= literal
s_error _handle_ld_imm_lit(s_program* program, s_asm_instruction* instruction)
{
    s_section* curr_section = get_current_section(program);
    s_machine_instruction machine_instr;

    long literal = instruction->load_store_literal;

    if (long_fits_in_12bits(literal))
    {
        machine_instr.operation_code = 0b1001;     // oc for storing data
        machine_instr.modifier = 0b0001;         // modification for A <= B + disp
        machine_instr.reg_a = instruction->reg1;
        machine_instr.reg_b = 0;
        machine_instr.reg_c = 0;
        machine_instr.displacement = literal;         

        write_machine_instr_to_section(curr_section, machine_instr);

        return new_no_error();
    }
    else
    {
        long section_offset = get_section_offset(curr_section);

        machine_instr.operation_code = 0b1001;     // oc for storing data
        machine_instr.modifier = 0b0010;         // modification for A < == mem[B + C + disp]
        machine_instr.reg_a = instruction->reg1 ;
        machine_instr.reg_b = ASM_REG_PC;
        machine_instr.reg_c = 0;
        machine_instr.displacement = 0;         // will be changed so it goes to the trampline

        write_machine_instr_to_section(curr_section, machine_instr);

        add_literal_trampoline_entry(program, curr_section, section_offset+2, section_offset, literal);

        return new_no_error();
    }
}

// reg1 <= symbol
s_error _handle_ld_imm_sym(s_program* program, s_asm_instruction* instruction)
{
    s_section* curr_section = get_current_section(program);
    s_machine_instruction machine_instr;

    string symbol_name = instruction->load_store_symbol;

    s_symbol_table_entry* symbol = get_and_create_new_symbol_entry(get_symbol_table(program), symbol_name);
    long symbol_index = get_symbol_entry_index_by_symbol(get_symbol_table(program), symbol_name);

    long section_offset = get_section_offset(curr_section);

    machine_instr.operation_code = 0b1001;     // oc for storing data
    machine_instr.modifier = 0b0010;         // modification for A < == mem[B + C + disp]
    machine_instr.reg_a = instruction->reg1;
    machine_instr.reg_b = ASM_REG_PC;
    machine_instr.reg_c = 0;
    machine_instr.displacement = 0;         // will be changed so it goes to the trampline

    write_machine_instr_to_section(curr_section, machine_instr);

    add_symbol_trampoline_entry(program, curr_section, section_offset+2, section_offset, symbol_index);

    return new_no_error();
}

// reg1 <= [literal]
s_error _handle_ld_mem_lit(s_program* program, s_asm_instruction* instruction)
{
    s_section* curr_section = get_current_section(program);
    s_machine_instruction machine_instr;

    long literal = instruction->load_store_literal;

    if (long_fits_in_12bits(literal))
    {
        machine_instr.operation_code = 0b1001;
        machine_instr.modifier = 0b0010;         // modification for A <= mem[B + C + disp]
        machine_instr.reg_a = instruction->reg1;
        machine_instr.reg_b = 0;
        machine_instr.reg_c = 0;
        machine_instr.displacement = literal;         

        write_machine_instr_to_section(curr_section, machine_instr);

        return new_no_error();
    }
    else
    {
        // reg1 <=  literal
        // reg1 <= [reg1]
        long section_offset = get_section_offset(curr_section);

        machine_instr.operation_code = 0b1001;     // oc for storing data
        machine_instr.modifier = 0b0001;         // modification for A <= B + disp
        machine_instr.reg_a = instruction->reg1;
        machine_instr.reg_b = ASM_REG_PC;
        machine_instr.reg_c = 0;
        machine_instr.displacement = 0;         // will be changed so it goes to the trampline

        write_machine_instr_to_section(curr_section, machine_instr);

        add_literal_trampoline_entry(program, curr_section, section_offset+2, section_offset, literal);

        machine_instr.operation_code = 0b1001;     // oc for storing data
        machine_instr.modifier = 0b0010;         // modification for A <= mem[B + C + disp]
        machine_instr.reg_a = instruction->reg1;
        machine_instr.reg_b = instruction->reg1;
        machine_instr.reg_c = 0;
        machine_instr.displacement = 0;

        write_machine_instr_to_section(curr_section, machine_instr);

        return new_no_error();
    }
}

// reg1 <= [symbol]
s_error _handle_ld_mem_sym(s_program* program, s_asm_instruction* instruction)
{
    // reg1 <= symbol
    // reg1 <= [reg1]

    s_section* curr_section = get_current_section(program);
    s_machine_instruction machine_instr;

    string symbol_name = instruction->load_store_symbol;

    s_symbol_table_entry* symbol = get_and_create_new_symbol_entry(get_symbol_table(program), symbol_name);
    long symbol_index = get_symbol_entry_index_by_symbol(get_symbol_table(program), symbol_name);

    long section_offset = get_section_offset(curr_section);

    machine_instr.operation_code = 0b1001;     // oc for storing data
    machine_instr.modifier = 0b0010;         // modification for A <= mem[B + C + disp]
    machine_instr.reg_a = instruction->reg1;
    machine_instr.reg_b = ASM_REG_PC;
    machine_instr.reg_c = 0;
    machine_instr.displacement = 0;         // will be changed so it goes to the trampline

    write_machine_instr_to_section(curr_section, machine_instr);

    add_symbol_trampoline_entry(program, curr_section, section_offset+2, section_offset, symbol_index);

    machine_instr.operation_code = 0b1001;     // oc for storing data
    machine_instr.modifier = 0b0010;         // modification for A <= mem[B + C + disp]
    machine_instr.reg_a = instruction->reg1;
    machine_instr.reg_b = instruction->reg1;
    machine_instr.reg_c = 0;
    machine_instr.displacement = 0;

    write_machine_instr_to_section(curr_section, machine_instr);

    return new_no_error();
}

// reg1 <= reg2
s_error _handle_ld_reg(s_program* program, s_asm_instruction* instruction)
{
    s_section* curr_section = get_current_section(program);
    s_machine_instruction machine_instr;

    machine_instr.operation_code = 0b1001;     // oc for loading data
    machine_instr.modifier = 0b0001;         // modification A <= B + disp
    machine_instr.reg_a = instruction->reg1;
    machine_instr.reg_b = instruction->load_store_register;
    machine_instr.reg_c = 0;
    machine_instr.displacement = 0;         

    write_machine_instr_to_section(curr_section, machine_instr);

    return new_no_error();
}

// reg1 <= [reg2]
s_error _handle_ld_reg_mem(s_program* program, s_asm_instruction* instruction)
{
    s_section* curr_section = get_current_section(program);
    s_machine_instruction machine_instr;

    machine_instr.operation_code = 0b1001;     // oc for storing data
    machine_instr.modifier = 0b0010;         // modification A <= mem[B + C + disp]
    machine_instr.reg_a = instruction->reg1;
    machine_instr.reg_b = 0;
    machine_instr.reg_c = instruction->load_store_register;
    machine_instr.displacement = 0;         

    write_machine_instr_to_section(curr_section, machine_instr);

    return new_no_error();
}

// reg1 <= [reg2 + lit]
s_error _handle_ld_reg_mem_lit(s_program* program, s_asm_instruction* instruction)
{
    s_section* curr_section = get_current_section(program);
    s_machine_instruction machine_instr;

    long literal = instruction->load_store_literal;

    if (long_fits_in_12bits(literal))
    {
        machine_instr.operation_code = 0b1001;     
        machine_instr.modifier = 0b0010;         // modification for A <= mem[B + C + disp]
        machine_instr.reg_a = instruction->reg1;
        machine_instr.reg_b = instruction->load_store_register;
        machine_instr.reg_c = 0;
        machine_instr.displacement = literal;         

        write_machine_instr_to_section(curr_section, machine_instr);

        return new_no_error();
    }
    else
    {
        return new_error(ERR_LITERAL_TOO_LARGE_LS);
    }
}

// reg2 <= [reg1 + sym]
s_error _handle_ld_reg_mem_sym(s_program* program, s_asm_instruction* instruction)
{
    s_section* curr_section = get_current_section(program);
    s_machine_instruction machine_instr;

    string symbol_name = instruction->load_store_symbol;
    s_symbol_table_entry* symbol = get_and_create_new_symbol_entry(get_symbol_table(program), symbol_name);

    long section_offset = get_section_offset(curr_section);

    machine_instr.operation_code = 0b1001;     
    machine_instr.modifier = 0b0010;         // modification for A <= mem[B + C + disp]
    machine_instr.reg_a = instruction->reg1;
    machine_instr.reg_b = 0;
    machine_instr.reg_c = instruction->load_store_register;
    machine_instr.displacement = 0;         

    write_machine_instr_to_section(curr_section, machine_instr);
    add_mem_reg_sym_to_symbol(symbol, curr_section, section_offset + 2, instruction);

    return new_no_error();
}
