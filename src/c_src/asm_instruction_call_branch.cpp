#include "asm_instruction.hpp"
#include "section.hpp"
#include "asm.hpp"
#include "error.hpp"
#include "machine_instruction.hpp"
#include "misc.hpp"
#include "trampoline.hpp"
#include "symbol_table.hpp"


s_error handle_call(s_asm_instruction* instruction)
{
    char oc = 0b0010, mod_12bit_fit = 0b0000, mod_12bit_no_fit = 0b0001;
    return _handle_jump_branch(instruction, oc, mod_12bit_fit, mod_12bit_no_fit);
}

s_error handle_jmp(s_asm_instruction* instruction)
{
    char oc = 0b0011, mod_12bit_fit = 0b0000, mod_12bit_no_fit = 0b1000;
    return _handle_jump_branch(instruction, oc, mod_12bit_fit, mod_12bit_no_fit);
}
s_error handle_beq(s_asm_instruction* instruction)
{
    char oc = 0b0011, mod_12bit_fit = 0b0001, mod_12bit_no_fit = 0b1001;
    return _handle_jump_branch(instruction, oc, mod_12bit_fit, mod_12bit_no_fit);
}
s_error handle_bne(s_asm_instruction* instruction)
{
    char oc = 0b0011, mod_12bit_fit = 0b0010, mod_12bit_no_fit = 0b1010;
    return _handle_jump_branch(instruction, oc, mod_12bit_fit, mod_12bit_no_fit);
}
s_error handle_bgt(s_asm_instruction* instruction)
{
    char oc = 0b0011, mod_12bit_fit = 0b0011, mod_12bit_no_fit = 0b1011;
    return _handle_jump_branch(instruction, oc, mod_12bit_fit, mod_12bit_no_fit);
}

s_error _handle_jump_branch(s_asm_instruction* instruction, char oc, char mod_12bit_fit, char mod_12bit_no_fit)
{
    s_section* curr_section = get_current_section();
    s_machine_instruction machine_instr;

    if (instruction->jump_branch_operand_type == ASM_OP_JMP_LIT)
    {
        // call literal
        if (long_fits_in_12bits(instruction->jump_branch_literal))
        {
            // fits in 12bits 
            // no need for literal pool
            machine_instr.operation_code = oc;
            machine_instr.modifier = mod_12bit_fit;
            machine_instr.reg_a = 0;
            machine_instr.reg_b = instruction->reg1;
            machine_instr.reg_c = instruction->reg2;
            machine_instr.displacement = instruction->jump_branch_literal;

            write_machine_instr_to_section(curr_section, machine_instr);

            return new_no_error();
        }
        else
        {
            // doesnt fit in 12bits
            // make entry in literal pool, trampoline
            long instr_pos = get_section_offset(curr_section);

            machine_instr.operation_code = oc;
            machine_instr.modifier = mod_12bit_no_fit;
            machine_instr.reg_a = ASM_REG_PC;
            machine_instr.reg_b = instruction->reg1;
            machine_instr.reg_c = instruction->reg2;
            machine_instr.displacement = 0;         // this will later be replaced by the offset from PC to trampoline
            
            write_machine_instr_to_section(curr_section, machine_instr);

            add_literal_trampoline_entry(curr_section, instr_pos + 2, instr_pos, instruction->jump_branch_literal);

            return new_no_error();
        }
    }
    else if (instruction->jump_branch_operand_type == ASM_OP_JMP_SYM)
    {
        s_symbol_table_entry* symbol = get_and_create_new_symbol_entry(instruction->jump_branch_symbol);
        long symbol_index = get_symbol_entry_index_by_symbol(symbol->name);

        // call symbol
        // doesnt fit in 12bits
        // make entry in literal pool, trampoline
        long instr_pos = get_section_offset(curr_section);

        machine_instr.operation_code = oc;
        machine_instr.modifier = mod_12bit_no_fit;
        machine_instr.reg_a = ASM_REG_PC;
        machine_instr.reg_b = instruction->reg1;
        machine_instr.reg_c = instruction->reg2;
        machine_instr.displacement = 0;         // this will later be replaced by the offset from PC to trampoline
            
        write_machine_instr_to_section(curr_section, machine_instr);

        add_symbol_trampoline_entry(curr_section, instr_pos + 2, instr_pos, symbol_index);

        return new_no_error();
    }
}