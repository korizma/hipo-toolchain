#include "timer.hpp"
#include "emu.hpp"
#include <thread>

int _sleep_ms(int config)
{
    switch (config)
    {
        case 0x0:
            return 500;
        case 0x1:
            return 1000;
        case 0x2:
            return 1500;
        case 0x3:
            return 2000;
        case 0x4:
            return 5000;
        case 0x5:
            return 10000;
        case 0x6:
            return 30000;
        case 0x7:
            return 60000;
    }
    return 1000;
}

void timer_loop(s_emu_state* state)
{
    int config, ms;
    while (state->running)
    {
        state->emu_mutex.lock();
        config = read_int_from_emu(state, TIMER_CONFIG);
        state->emu_mutex.unlock();

        ms = _sleep_ms(config);
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
        state->timer_interrupt = true;

        while (state->timer_interrupt & state->running);
    }
}