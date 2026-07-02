#pragma once

#include <vector>
#include <string>

using namespace std;

// disable echo and restore terminal functions
struct termios;
struct s_emu_state;

int disable_echo(struct termios *old_termios);

int restore_terminal(const struct termios *old_termios);

#define TERM_IN 0xFFFFFF00
#define TERM_OUT 0xFFFFFF04

void output_terminal_loop(s_emu_state* state);

void input_terminal_loop(s_emu_state* state);