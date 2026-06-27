#pragma once

#include <string>
#include <vector>

struct s_section;

using namespace std;

typedef struct s_machine_instruction
{
    char operation_code;
    char modifier;
    char reg_a, reg_b, reg_c;
    int displacement;
} s_machine_instruction;

vector<char> machine_instruction_to_bytes(s_machine_instruction instr);

void write_machine_instr_to_section(s_section* section, s_machine_instruction instr);