#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <mutex>

struct s_machine_instruction;

using namespace std;

#define ENTRY_LOCATION 0x40000000
#define REG_NUMBER 16
#define CONTROL_REG_NUMBER 3
#define SLEEP_FOR_SYNC 2000

struct s_hipo_state;

typedef struct s_emu_state
{
    unordered_map<long, char> bytes;
    vector<int> regs;
    vector<int> control_regs;

    bool running;

    bool term_in_interrupt;
    bool timer_interrupt;

    mutex emu_mutex;
} s_emu_state;

s_emu_state* new_emu_state(unordered_map<long, char> bytes);

unordered_map<long, char> load_bytes_from_file_emu(string filename);

vector<char> read_4_bytes_from_emu(s_emu_state* emu_state, long address);

int read_int_from_emu(s_emu_state* emu_state, long address);

void write_int_to_emu(s_emu_state* emu_state, long address, int value);

void emulate_file(string filename);

s_machine_instruction read_machine_instruction(s_emu_state* emu_state, long address);

string registers_to_string(s_emu_state* emu_state);

bool blocking_all_interrupts(s_emu_state* state);

bool blocking_timer_interrupts(s_emu_state* state);

bool blocking_terminal_interrupts(s_emu_state* state);

void call_interrupt_routine(s_emu_state* state, int reason);