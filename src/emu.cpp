#include "emu.hpp"
#include "asm_instruction.hpp"
#include "machine_instruction.hpp"
#include "misc.hpp"
#include <cstdint>
#include <iostream>
#include <thread>
#include "terminal.hpp"
#include "timer.hpp"
#include <termios.h>

using namespace std;

s_emu_state* new_emu_state(unordered_map<long, char> bytes)
{
    s_emu_state* state = new s_emu_state();
    state->bytes = bytes;
    state->regs = vector<int>(REG_NUMBER, 0);
    state->regs[ASM_REG_PC] = ENTRY_LOCATION;
    state->running = true;
    state->control_regs = vector<int>(CONTROL_REG_NUMBER, 0);
    state->term_in_interrupt = false;
    state->timer_interrupt = false;
    return state;
}

unordered_map<long, char> load_bytes_from_file_emu(string filename)
{
    string text = read_all_text(filename);
    
    vector<string> lines = split_string(text, '\n');

    unordered_map<long, char> memory_map;

    for (string line : lines)
    {
        line = trim_string(line);
        if (line == "")
            continue;
        vector<string> addr_chars = split_string(line, ':');
        
        long address = stoi(addr_chars[0], nullptr, 16);
        vector<string> chars = split_string(trim_string(addr_chars[1]), ' ');

        for (int i = 0; i < chars.size(); i++)
        {
            memory_map[address + i] = string_hex_to_char(trim_string(chars[i]));
        }
    }

    return memory_map;
}

vector<char> read_4_bytes_from_emu(s_emu_state* emu_state, long address)
{
    vector<char> bytes(4);
    for (int i = 0; i < 4; i++)
    {
        bytes[i] = emu_state->bytes[static_cast<uint32_t>(address) + i];
    }
    return bytes;
}

int read_int_from_emu(s_emu_state* emu_state, long address)
{
    uint32_t value = 0;
    for (int i = 0; i < 4; i++)
    {
        value |= (static_cast<uint32_t>(static_cast<unsigned char>(emu_state->bytes[static_cast<uint32_t>(address) + i])) << (8 * i));
    }
    return static_cast<int>(value);
}

s_machine_instruction read_machine_instruction(s_emu_state* emu_state, long address)
{
    vector<char> bytes = read_4_bytes_from_emu(emu_state, address);
    return bytes_to_machine_instruction(bytes);
}

void emulate_file(string filename)
{
    unordered_map<long, char> bytes = load_bytes_from_file_emu(filename);
    s_emu_state* emu_state = new_emu_state(bytes);

    termios oldt;
    disable_echo(&oldt);

    thread terminal_in(input_terminal_loop, emu_state);
    thread timer(timer_loop, emu_state);

    while (emu_state->running)
    {
        emu_state->emu_mutex.lock();
        if (!blocking_all_interrupts(emu_state))
        {
            if (!blocking_terminal_interrupts(emu_state) && emu_state->term_in_interrupt)
            {
                call_interrupt_routine(emu_state, 3);
                emu_state->term_in_interrupt = false;
            }
            else if (!blocking_timer_interrupts(emu_state) && emu_state->timer_interrupt)
            {
                call_interrupt_routine(emu_state, 2);
                emu_state->timer_interrupt = false;
            }
        }

        long pc = emu_state->regs[ASM_REG_PC];
        s_machine_instruction instr = read_machine_instruction(emu_state, pc);
        execute_machine_instr(emu_state, instr);
        emu_state->emu_mutex.unlock();
    }

    terminal_in.detach();
    timer.join();

    restore_terminal(&oldt);

    cout << registers_to_string(emu_state);
}

string registers_to_string(s_emu_state* emu_state)
{
    string result = "";
    for (int i = 0; i < emu_state->regs.size(); i++)
    {
        result += "r" + to_string(i) + ": " + int_to_string_hex(emu_state->regs[i]) + "\n";
    }
    return result;
}

void write_int_to_emu(s_emu_state* emu_state, long address, int value)
{
    uint32_t addr = static_cast<uint32_t>(address);
    if (addr == TERM_OUT) 
    {
        putc(value & 0xFF, stdout);
        fflush(stdout);
    }
    vector<char> bytes = int_to_4_bytes(value);
    for (int i = 0; i < 4; i++)
    {
        emu_state->bytes[addr + i] = bytes[i];
    }
}


bool blocking_all_interrupts(s_emu_state* state)
{
    return (state->control_regs[ASM_REG_STATUS] & 0b100) != 0;
}

bool blocking_timer_interrupts(s_emu_state* state)
{
    return (state->control_regs[ASM_REG_STATUS] & 0b001) != 0;
}
 
bool blocking_terminal_interrupts(s_emu_state* state)
{
    return (state->control_regs[ASM_REG_STATUS] & 0b010) != 0;
}

void call_interrupt_routine(s_emu_state* state, int reason)
{
    int sp = state->regs[ASM_REG_SP];
    write_int_to_emu(state, sp - 8, state->regs[ASM_REG_PC]);
    write_int_to_emu(state, sp - 4, state->control_regs[ASM_REG_STATUS]);
    state->regs[ASM_REG_SP] = sp - 8;
    state->control_regs[ASM_REG_CAUSE] = reason;
    state->control_regs[ASM_REG_STATUS] = 0b111;
    state->regs[ASM_REG_PC] = state->control_regs[ASM_REG_HANDLER];
}
