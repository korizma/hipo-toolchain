#pragma once

#include <string>

using namespace std;

struct s_emu_state;

#define TIMER_CONFIG ((uint32_t)0xFFFFFF10)

void timer_loop(s_emu_state* state);