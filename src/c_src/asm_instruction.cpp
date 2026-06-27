#include "asm_instruction.hpp"
#include "section.hpp"
#include "asm.hpp"
#include "error.hpp"
#include "machine_instruction.hpp"

s_asm_instruction* new_asm_instruction()
{
    return (s_asm_instruction*)malloc(sizeof(s_asm_instruction));
}

s_error handle_asm_instruction(s_asm_instruction* instruction)
{
    s_section* curr_section = get_current_section();

    if (curr_section == NULL)
        return new_error(ERR_LINE_OUTSIDE_SECTION);

    s_error err;
#include "error.hpp"
    switch (instruction->instruction)
    {
    case ASM_INSTR_HALT:
        err = handle_halt(instruction);
        break;
    case ASM_INSTR_INT:
        err = handle_int(instruction);
        break;
    case ASM_INSTR_IRET:
        err = handle_iret(instruction);
        break;
    case ASM_INSTR_CALL:
        err = handle_call(instruction);
        break;
    case ASM_INSTR_RET:
        err = handle_ret(instruction);
        break;
    case ASM_INSTR_PUSH:
        err = handle_push(instruction);
        break;
    case ASM_INSTR_POP:
        err = handle_pop(instruction);
        break;
    case ASM_INSTR_JMP:
        err = handle_jmp(instruction);
        break;
    case ASM_INSTR_BEQ:
        err = handle_beq(instruction);
        break;
    case ASM_INSTR_BNE:
        err = handle_bne(instruction);
        break;
    case ASM_INSTR_BGT:
        err = handle_bgt(instruction);
        break;
    case ASM_INSTR_XCHG:
        err = handle_xchg(instruction);
        break;
    case ASM_INSTR_ADD:
        err = handle_add(instruction);
        break;
    case ASM_INSTR_SUB:
        err = handle_sub(instruction);
        break;
    case ASM_INSTR_MUL:
        err = handle_mul(instruction);
        break;
    case ASM_INSTR_DIV:
        err = handle_div(instruction);
        break;
    case ASM_INSTR_NOT:
        err = handle_not(instruction);
        break;
    case ASM_INSTR_AND:
        err = handle_and(instruction);
        break;
    case ASM_INSTR_OR:
        err = handle_or(instruction);
        break;
    case ASM_INSTR_XOR:
        err = handle_xor(instruction);
        break;
    case ASM_INSTR_SHL:
        err = handle_shl(instruction);
        break;
    case ASM_INSTR_SHR:
        err = handle_shr(instruction);
        break;
    case ASM_INSTR_LD:
        err = handle_ld(instruction);
        break;
    case ASM_INSTR_ST:
        err = handle_st(instruction);
        break;
    case ASM_INSTR_CSRRD:
        err = handle_csrrd(instruction);
        break;
    case ASM_INSTR_CSRWR:
        err = handle_csrwr(instruction);
        break;
    
    default:
        break;
    }

    return err;
}

string asm_instruction_to_string(s_asm_instruction* instruction)\
{
    return "";
}

s_error handle_halt(s_asm_instruction* instruction)
{
    s_section* curr_section = get_current_section();
    s_machine_instruction machine_instr;

    machine_instr.operation_code = 0;   // oc for HALT instruction
    machine_instr.modifier = 0;
    machine_instr.reg_a = 0;
    machine_instr.reg_b = 0;
    machine_instr.reg_c = 0;
    machine_instr.displacement = 0;

    write_machine_instr_to_section(curr_section, machine_instr);

    return new_no_error();
}
s_error handle_int(s_asm_instruction* instruction)
{
    s_section* curr_section = get_current_section();
    s_machine_instruction machine_instr;

    machine_instr.operation_code = 1;   // oc for INT instruction
    machine_instr.modifier = 0;
    machine_instr.reg_a = 0;
    machine_instr.reg_b = 0;
    machine_instr.reg_c = 0;
    machine_instr.displacement = 0;

    write_machine_instr_to_section(curr_section, machine_instr);

    return new_no_error();
}
s_error handle_iret(s_asm_instruction* instruction)
{
    // iret <=> pop status, pop pc
    // if pc was popped first then status would never be popped
    s_section* curr_section = get_current_section();
    s_machine_instruction machine_instr;

    // pop status
    machine_instr.operation_code = 0b1001;   // oc for loading data instruction
    machine_instr.modifier = 0b0111;        // modification for csrA <= mem32[B], B <= B + D
    machine_instr.reg_a = ASM_REG_STATUS;
    machine_instr.reg_b = ASM_REG_SP;
    machine_instr.reg_c = 0;
    machine_instr.displacement = 4;         // we add the stack size

    write_machine_instr_to_section(curr_section, machine_instr);

    // pop pc
    machine_instr.operation_code = 0b1001;   // oc for loading data instruction
    machine_instr.modifier = 0b0011;        // modification for A <= mem32[B], B <= B + D
    machine_instr.reg_a = ASM_REG_PC;
    machine_instr.reg_b = ASM_REG_SP;
    machine_instr.reg_c = 0;
    machine_instr.displacement = 4;         // we add the stack size

    write_machine_instr_to_section(curr_section, machine_instr);

    return new_no_error();
}

s_error handle_call(s_asm_instruction* instruction); // for later
s_error handle_ret(s_asm_instruction* instruction)
{
    // ret <=> pop pc
    s_section* curr_section = get_current_section();
    s_machine_instruction machine_instr;

    // pop pc
    machine_instr.operation_code = 0b1001;   // oc for loading data instruction
    machine_instr.modifier = 0b0011;        // modification for A <= mem32[B], B <= B + D
    machine_instr.reg_a = ASM_REG_PC;
    machine_instr.reg_b = ASM_REG_SP;
    machine_instr.reg_c = 0;
    machine_instr.displacement = 4;         // we add the stack size

    write_machine_instr_to_section(curr_section, machine_instr);

    return new_no_error();
}
s_error handle_push(s_asm_instruction* instruction)
{
    // push reg <=> sp = sp - 4, mem[sp] <= reg
    s_section* curr_section = get_current_section();
    s_machine_instruction machine_instr;

    // sp = sp - 4
    machine_instr.operation_code = 0b1001;   // oc for loading data instruction
    machine_instr.modifier = 0b0001;        // modification for A <= B + D
    machine_instr.reg_a = ASM_REG_SP;
    machine_instr.reg_b = ASM_REG_SP;
    machine_instr.reg_c = 0;
    machine_instr.displacement = -4;         // add -4 to the sp

    write_machine_instr_to_section(curr_section, machine_instr);

    // mem[sp] <= reg1
    machine_instr.operation_code = 0b1000;   // oc for storing data instruction
    machine_instr.modifier = 0b0000;        // modification for mem32[A + B + D] <= C
    machine_instr.reg_a = ASM_REG_SP;
    machine_instr.reg_b = 0;
    machine_instr.reg_c = 0;
    machine_instr.displacement = 0;         

    write_machine_instr_to_section(curr_section, machine_instr);

    return new_no_error();
}
s_error handle_pop(s_asm_instruction* instruction)
{
    // pop reg <=> reg <= mem[SP], SP = SP + 4
    s_section* curr_section = get_current_section();
    s_machine_instruction machine_instr;

    // pop pc
    machine_instr.operation_code = 0b1001;   // oc for loading data instruction
    machine_instr.modifier = 0b0011;        // modification for A <= mem32[B], B <= B + D
    machine_instr.reg_a = instruction->reg1;
    machine_instr.reg_b = ASM_REG_SP;
    machine_instr.reg_c = 0;
    machine_instr.displacement = 4;         // we add the stack size

    write_machine_instr_to_section(curr_section, machine_instr);

    return new_no_error();
}


s_error handle_jmp(s_asm_instruction* instruction);
s_error handle_beq(s_asm_instruction* instruction);
s_error handle_bne(s_asm_instruction* instruction);
s_error handle_bgt(s_asm_instruction* instruction);

s_error handle_xchg(s_asm_instruction* instruction)
{
    s_section* curr_section = get_current_section();
    s_machine_instruction machine_instr;

    machine_instr.operation_code = 0b0100;   // oc for atomic data swap
    machine_instr.modifier = 0b0000;        
    machine_instr.reg_a = 0;
    machine_instr.reg_b = instruction->reg1;
    machine_instr.reg_c = instruction->reg2;
    machine_instr.displacement = 0;         

    write_machine_instr_to_section(curr_section, machine_instr);

    return new_no_error();
}

s_error handle_add(s_asm_instruction* instruction)
{
    s_section* curr_section = get_current_section();
    s_machine_instruction machine_instr;

    machine_instr.operation_code = 0b0101;   // oc for arithmetic operations
    machine_instr.modifier = 0b0000;         // modification for addition, A <= B + C
    machine_instr.reg_a = instruction->reg2;
    machine_instr.reg_b = instruction->reg1;
    machine_instr.reg_c = instruction->reg2;
    machine_instr.displacement = 0;         

    write_machine_instr_to_section(curr_section, machine_instr);

    return new_no_error();
}
s_error handle_sub(s_asm_instruction* instruction)
{
    s_section* curr_section = get_current_section();
    s_machine_instruction machine_instr;

    machine_instr.operation_code = 0b0101;   // oc for arithmetic operations
    machine_instr.modifier = 0b0001;         // modification for subtraction, A <= B - C
    machine_instr.reg_a = instruction->reg2;
    machine_instr.reg_b = instruction->reg1;
    machine_instr.reg_c = instruction->reg2;
    machine_instr.displacement = 0;         

    write_machine_instr_to_section(curr_section, machine_instr);

    return new_no_error();
}
s_error handle_mul(s_asm_instruction* instruction)
{
    s_section* curr_section = get_current_section();
    s_machine_instruction machine_instr;

    machine_instr.operation_code = 0b0101;   // oc for arithmetic operations
    machine_instr.modifier = 0b0010;         // modification for multiplication, A <= B * C
    machine_instr.reg_a = instruction->reg2;
    machine_instr.reg_b = instruction->reg1;
    machine_instr.reg_c = instruction->reg2;
    machine_instr.displacement = 0;         

    write_machine_instr_to_section(curr_section, machine_instr);

    return new_no_error();
}
s_error handle_div(s_asm_instruction* instruction)
{
    s_section* curr_section = get_current_section();
    s_machine_instruction machine_instr;

    machine_instr.operation_code = 0b0101;   // oc for arithmetic operations
    machine_instr.modifier = 0b0011;         // modification for division, A <= B / C
    machine_instr.reg_a = instruction->reg2;
    machine_instr.reg_b = instruction->reg1;
    machine_instr.reg_c = instruction->reg2;
    machine_instr.displacement = 0;         

    write_machine_instr_to_section(curr_section, machine_instr);

    return new_no_error();
}

s_error handle_not(s_asm_instruction* instruction)
{
    s_section* curr_section = get_current_section();
    s_machine_instruction machine_instr;

    machine_instr.operation_code = 0b0110;   // oc for logical operations
    machine_instr.modifier = 0b0000;         // modification for not, A <= ~B
    machine_instr.reg_a = instruction->reg1;
    machine_instr.reg_b = instruction->reg1;
    machine_instr.reg_c = 0;
    machine_instr.displacement = 0;         

    write_machine_instr_to_section(curr_section, machine_instr);

    return new_no_error();
}

s_error handle_and(s_asm_instruction* instruction)
{
    s_section* curr_section = get_current_section();
    s_machine_instruction machine_instr;

    machine_instr.operation_code = 0b0110;   // oc for logical operations
    machine_instr.modifier = 0b0001;         // modification for and, A <= B & C
    machine_instr.reg_a = instruction->reg2;
    machine_instr.reg_b = instruction->reg1;
    machine_instr.reg_c = instruction->reg2;
    machine_instr.displacement = 0;         

    write_machine_instr_to_section(curr_section, machine_instr);

    return new_no_error();
}
s_error handle_or(s_asm_instruction* instruction)
{
    s_section* curr_section = get_current_section();
    s_machine_instruction machine_instr;

    machine_instr.operation_code = 0b0110;   // oc for logical operations
    machine_instr.modifier = 0b0010;         // modification for or, A <= B | C
    machine_instr.reg_a = instruction->reg2;
    machine_instr.reg_b = instruction->reg1;
    machine_instr.reg_c = instruction->reg2;
    machine_instr.displacement = 0;         

    write_machine_instr_to_section(curr_section, machine_instr);

    return new_no_error();
}
s_error handle_xor(s_asm_instruction* instruction)
{
    s_section* curr_section = get_current_section();
    s_machine_instruction machine_instr;

    machine_instr.operation_code = 0b0110;   // oc for logical operations
    machine_instr.modifier = 0b0011;         // modification for xor, A <= B ^ C
    machine_instr.reg_a = instruction->reg2;
    machine_instr.reg_b = instruction->reg1;
    machine_instr.reg_c = instruction->reg2;
    machine_instr.displacement = 0;         

    write_machine_instr_to_section(curr_section, machine_instr);

    return new_no_error();
}
s_error handle_shl(s_asm_instruction* instruction)
{
    s_section* curr_section = get_current_section();
    s_machine_instruction machine_instr;

    machine_instr.operation_code = 0b0111;   // oc for shift operations
    machine_instr.modifier = 0b0000;         // modification for left shift, A <= B << C
    machine_instr.reg_a = instruction->reg2;
    machine_instr.reg_b = instruction->reg2;
    machine_instr.reg_c = instruction->reg1;
    machine_instr.displacement = 0;         

    write_machine_instr_to_section(curr_section, machine_instr);

    return new_no_error();
}
s_error handle_shr(s_asm_instruction* instruction)
{
    s_section* curr_section = get_current_section();
    s_machine_instruction machine_instr;

    machine_instr.operation_code = 0b0111;   // oc for shift operations
    machine_instr.modifier = 0b0001;         // modification for right shift, A <= B >> C
    machine_instr.reg_a = instruction->reg2;
    machine_instr.reg_b = instruction->reg2;
    machine_instr.reg_c = instruction->reg1;
    machine_instr.displacement = 0;         

    write_machine_instr_to_section(curr_section, machine_instr);

    return new_no_error();
}

s_error handle_ld(s_asm_instruction* instruction);

s_error handle_st(s_asm_instruction* instruction);

s_error handle_csrrd(s_asm_instruction* instruction)
{
    s_section* curr_section = get_current_section();
    s_machine_instruction machine_instr;

    machine_instr.operation_code = 0b1001;   // oc for loading operations
    machine_instr.modifier = 0b0000;         // modification for gprA <= csrB
    machine_instr.reg_a = instruction->reg2;
    machine_instr.reg_b = instruction->control_reg;
    machine_instr.reg_c = 0;
    machine_instr.displacement = 0;         

    write_machine_instr_to_section(curr_section, machine_instr);

    return new_no_error();
}
s_error handle_csrwr(s_asm_instruction* instruction)
{
    s_section* curr_section = get_current_section();
    s_machine_instruction machine_instr;

    machine_instr.operation_code = 0b1001;   // oc for loading operations
    machine_instr.modifier = 0b0100;         // modification for csrA <= gprB
    machine_instr.reg_a = instruction->control_reg;
    machine_instr.reg_b = instruction->reg1;
    machine_instr.reg_c = 0;
    machine_instr.displacement = 0;         

    write_machine_instr_to_section(curr_section, machine_instr);

    return new_no_error();
}