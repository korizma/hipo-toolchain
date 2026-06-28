#include "machine_instruction.hpp"
#include "section.hpp"

using namespace std;

vector<char> machine_instruction_to_bytes(s_machine_instruction instr)
{
    char b1, b2, b3, b4;
    b1 = ((instr.operation_code & 0xF) << 4 ) | (instr.modifier & 0xF);
    b2 = ((instr.reg_a & 0xF) << 4 ) | (instr.reg_b & 0xF);
    b3 = ((instr.reg_c & 0xF) << 4 ) | (instr.displacement & 0xF);
    b4 = (instr.displacement >> 4) & 0xFF;

    vector<char> bytes  = {b1, b2, b3, b4};
    return bytes;
}

void write_machine_instr_to_section(s_section* section, s_machine_instruction instr)
{
    vector<char> bytes = machine_instruction_to_bytes(instr);
    write_bytes_to_section(section, bytes);
}