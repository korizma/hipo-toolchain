#pragma once

#include <string>
#include <vector>
#include "emu.hpp"

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

s_machine_instruction bytes_to_machine_instruction(vector<char> bytes);

void write_machine_instr_to_section(s_section* section, s_machine_instruction instr);

bool execute_machine_instr(s_emu_state* state, s_machine_instruction instr);

bool handle_oc_0000(s_emu_state* state, s_machine_instruction instr);
bool handle_oc_0001(s_emu_state* state, s_machine_instruction instr);
bool handle_oc_0010(s_emu_state* state, s_machine_instruction instr);
bool handle_oc_0011(s_emu_state* state, s_machine_instruction instr);
bool handle_oc_0100(s_emu_state* state, s_machine_instruction instr);
bool handle_oc_0101(s_emu_state* state, s_machine_instruction instr);
bool handle_oc_0110(s_emu_state* state, s_machine_instruction instr);
bool handle_oc_0111(s_emu_state* state, s_machine_instruction instr);
bool handle_oc_1000(s_emu_state* state, s_machine_instruction instr);
bool handle_oc_1001(s_emu_state* state, s_machine_instruction instr);