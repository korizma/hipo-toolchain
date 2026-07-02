#include "machine_instruction.hpp"
#include "section.hpp"
#include "emu.hpp"
#include "asm_instruction.hpp"

using namespace std;

static int sign_extend_12bit(int value)
{
    value &= 0xFFF;
    if (value & 0x800)
        value |= ~0xFFF;
    return value;
}

vector<char> machine_instruction_to_bytes(s_machine_instruction instr)
{
    char b1, b2, b3, b4;
    int displacement = instr.displacement & 0xFFF;
    b1 = ((instr.operation_code & 0xF) << 4 ) | (instr.modifier & 0xF);
    b2 = ((instr.reg_a & 0xF) << 4 ) | (instr.reg_b & 0xF);
    b3 = ((instr.reg_c & 0xF) << 4 ) | (displacement & 0xF);
    b4 = (displacement >> 4) & 0xFF;

    vector<char> bytes  = {b1, b2, b3, b4};
    return bytes;
}

void write_machine_instr_to_section(s_section* section, s_machine_instruction instr)
{
    vector<char> bytes = machine_instruction_to_bytes(instr);
    write_bytes_to_section(section, bytes);
}


s_machine_instruction bytes_to_machine_instruction(vector<char> bytes)
{
    s_machine_instruction instr;
    unsigned char b0 = static_cast<unsigned char>(bytes[0]);
    unsigned char b1 = static_cast<unsigned char>(bytes[1]);
    unsigned char b2 = static_cast<unsigned char>(bytes[2]);
    unsigned char b3 = static_cast<unsigned char>(bytes[3]);

    instr.operation_code = (b0 >> 4) & 0xF;
    instr.modifier = b0 & 0xF;
    instr.reg_a = (b1 >> 4) & 0xF;
    instr.reg_b = b1 & 0xF;
    instr.reg_c = (b2 >> 4) & 0xF;
    instr.displacement = sign_extend_12bit((b3 << 4) | (b2 & 0xF));
    return instr;
}
void execute_machine_instr(s_emu_state* state, s_machine_instruction instr)
{
    switch (instr.operation_code)
    {
        case 0x0:
            handle_oc_0000(state, instr);
            break;
        case 0x1:
            handle_oc_0001(state, instr);
            break;
        case 0x2:
            handle_oc_0010(state, instr);
            break;
        case 0x3:
            handle_oc_0011(state, instr);
            break;
        case 0x4:
            handle_oc_0100(state, instr);
            break;
        case 0x5:
            handle_oc_0101(state, instr);
            break;
        case 0x6:
            handle_oc_0110(state, instr);
            break;
        case 0x7:
            handle_oc_0111(state, instr);
            break;
        case 0x8:
            handle_oc_1000(state, instr);
            break;
        case 0x9:
            handle_oc_1001(state, instr);
            break;
    }
}

void handle_oc_0000(s_emu_state* state, s_machine_instruction instr)
{
    state->running = false;
    state->regs[ASM_REG_PC] += 4;
}

void handle_oc_0001(s_emu_state* state, s_machine_instruction instr)
{
    int sp = state->regs[ASM_REG_SP];
    write_int_to_emu(state, sp - 4, state->regs[ASM_REG_PC] + 4);
    write_int_to_emu(state, sp - 8, state->control_regs[ASM_REG_STATUS]);
    state->regs[ASM_REG_SP] = sp - 8;
    state->control_regs[ASM_REG_CAUSE] = 4;
    state->control_regs[ASM_REG_STATUS] &= (~0x1);
    state->regs[ASM_REG_PC] = state->control_regs[ASM_REG_HANDLER];
    state->regs[ASM_REG_PC] += 4;
}

void handle_oc_0010(s_emu_state* state, s_machine_instruction instr)
{
    int sp = state->regs[ASM_REG_SP];
    write_int_to_emu(state, sp - 4, state->regs[ASM_REG_PC] + 4);
    state->regs[ASM_REG_SP] = sp - 4;

    if (instr.modifier == 0b0000)
    {
        state->regs[ASM_REG_PC] = state->regs[instr.reg_a] + state->regs[instr.reg_b] + instr.displacement;
    }
    else if (instr.modifier == 0b0001)
    {
        int location = state->regs[instr.reg_a] + state->regs[instr.reg_b] + instr.displacement;
        state->regs[ASM_REG_PC] = read_int_from_emu(state, location);
    }
}
void handle_oc_0011(s_emu_state* state, s_machine_instruction instr)
{
    int location = state->regs[ASM_REG_PC] + 4;

    switch (instr.modifier)
    {
    case 0b0000:
        location = state->regs[instr.reg_a] + instr.displacement;
        break;
    
    case 0b0001:
        if (state->regs[instr.reg_b] == state->regs[instr.reg_c])
            location = state->regs[instr.reg_a] + instr.displacement;
        break;

    case 0b0010:
        if (state->regs[instr.reg_b] != state->regs[instr.reg_c])
            location = state->regs[instr.reg_a] + instr.displacement;
        break;

    case 0b0011:
        if (state->regs[instr.reg_b] > state->regs[instr.reg_c])
            location = state->regs[instr.reg_a] + instr.displacement;
        break;

    case 0b1000:
        location = state->regs[instr.reg_a] + instr.displacement;
        location = read_int_from_emu(state, location);
        break;
    
    case 0b1001:
        if (state->regs[instr.reg_b] == state->regs[instr.reg_c])
        {
            location = state->regs[instr.reg_a] + instr.displacement;
            location = read_int_from_emu(state, location);
        }
        break;

    case 0b1010:
        if (state->regs[instr.reg_b] != state->regs[instr.reg_c])
        {
            location = state->regs[instr.reg_a] + instr.displacement;
            location = read_int_from_emu(state, location);
        }
        break;

    case 0b1011:
        if (state->regs[instr.reg_b] > state->regs[instr.reg_c])
        {
            location = state->regs[instr.reg_a] + instr.displacement;
            location = read_int_from_emu(state, location);
        }
        break;
    }
    state->regs[ASM_REG_PC] = location;
}

void handle_oc_0100(s_emu_state* state, s_machine_instruction instr)
{
    int tmp = state->regs[instr.reg_b];
    state->regs[instr.reg_b] = state->regs[instr.reg_c];
    state->regs[instr.reg_c] = tmp;
    state->regs[ASM_REG_PC] += 4;
}

void handle_oc_0101(s_emu_state* state, s_machine_instruction instr)
{
    int b = state->regs[instr.reg_b], c = state->regs[instr.reg_c];
    state->regs[ASM_REG_PC] += 4;

    switch (instr.modifier)
    {
    case 0b0000:
        state->regs[instr.reg_a] = b + c;
        break;

    case 0b0001:
        state->regs[instr.reg_a] = b - c;
        break;
    
    case 0b0010:
        state->regs[instr.reg_a] = b * c;
        break;

    case 0b0011:
        state->regs[instr.reg_a] = b / c;
        break;
    }
}

void handle_oc_0110(s_emu_state* state, s_machine_instruction instr)
{
    int b = state->regs[instr.reg_b], c = state->regs[instr.reg_c];
    state->regs[ASM_REG_PC] += 4;

    switch (instr.modifier)
    {
    case 0b0000:
        state->regs[instr.reg_a] = ~b;
        break;

    case 0b0001:
        state->regs[instr.reg_a] = b & c;
        break;
    
    case 0b0010:
        state->regs[instr.reg_a] = b | c;
        break;

    case 0b0011:
        state->regs[instr.reg_a] = b ^ c;
        break;
    }
}

void handle_oc_0111(s_emu_state* state, s_machine_instruction instr)
{
    int b = state->regs[instr.reg_b], c = state->regs[instr.reg_c];
    state->regs[ASM_REG_PC] += 4;

    switch (instr.modifier)
    {
    case 0b0000:
        state->regs[instr.reg_a] = b << c;
        break;

    case 0b0001:
        state->regs[instr.reg_a] = b >> c;
        break;
    }
}

void handle_oc_1000(s_emu_state* state, s_machine_instruction instr)
{
    int a = state->regs[instr.reg_a], b = state->regs[instr.reg_b], c = state->regs[instr.reg_c];
    state->regs[ASM_REG_PC] += 4;
    int value;
    switch (instr.modifier)
    {
        case 0b0000:
            write_int_to_emu(state, a + b + instr.displacement, c);
            break;
        case 0b0010:
            value = read_int_from_emu(state, a + b + instr.displacement);
            write_int_to_emu(state, value, c);
            break;
        case 0b0001:
            state->regs[instr.reg_a] = a + instr.displacement;
            write_int_to_emu(state, a + instr.displacement, c);
            break;
    }
}

void handle_oc_1001(s_emu_state* state, s_machine_instruction instr)
{
    int a = state->regs[instr.reg_a], b = state->regs[instr.reg_b], c = state->regs[instr.reg_c];
    int value;
    state->regs[ASM_REG_PC] += 4;
    switch (instr.modifier)
    {
        case 0b0000:
            state->regs[instr.reg_a] = state->control_regs[instr.reg_b];
            break;
        case 0b0001:
            state->regs[instr.reg_a] = b + instr.displacement;
            break;
        case 0b0010:
            value = read_int_from_emu(state, b + c + instr.displacement);
            state->regs[instr.reg_a] = value;
            break;
        case 0b0011:
            value = read_int_from_emu(state, b);
            state->regs[instr.reg_a] = value;
            state->regs[instr.reg_b] = b + instr.displacement;
            break;
        case 0b0100:
            state->control_regs[instr.reg_a] = b;
            break;
        case 0b0101:
            state->control_regs[instr.reg_a] = state->control_regs[instr.reg_b] | instr.displacement;
            break;
        case 0b0110:
            value = read_int_from_emu(state, b + c + instr.displacement);
            state->control_regs[instr.reg_a] = value;
            break;
        case 0b0111:
            value = read_int_from_emu(state, b);
            state->control_regs[instr.reg_a] = value;
            state->regs[instr.reg_b] = b + instr.displacement;
            break;
    }
}
