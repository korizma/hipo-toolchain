#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <chrono>
#include <thread>
#include "emu.hpp"
#include "terminal.hpp"
#include <unistd.h>
#include <poll.h>

int disable_echo(struct termios *old_termios)
{
    struct termios new_termios;

    if (tcgetattr(STDIN_FILENO, old_termios) == -1) {
        perror("tcgetattr");
        return -1;
    }

    new_termios = *old_termios;
    new_termios.c_lflag &= ~(ICANON | ECHO);

    if (tcsetattr(STDIN_FILENO, TCSANOW, &new_termios) == -1) {
        perror("tcsetattr");
        return -1;
    }

    return 0;
}

int restore_terminal(const struct termios *old_termios)
{
    if (tcsetattr(STDIN_FILENO, TCSANOW, old_termios) == -1) {
        perror("tcsetattr");
        return -1;
    }

    return 0;
}


int getch_timeout(int ms)
{
    // STDIN_FILENO - what file is the poll waiting 
    // POLLIN - what the poll is waiting for
    pollfd pfd{STDIN_FILENO, POLLIN, 0};

    if (poll(&pfd, 1, ms) <= 0)
        return -1; // timeout or error

    unsigned char c;
    return read(STDIN_FILENO, &c, 1) == 1 ? c : -1;
}

void output_terminal_loop(s_emu_state* state)
{
    int character, prev;
    while (state->running)
    {
        state->emu_mutex.lock();
        character = read_int_from_emu(state, TERM_OUT);
        state->emu_mutex.unlock();

        if (character != prev)
            putc(character, stdout);
        prev = character;
    }
}

void input_terminal_loop(s_emu_state* state)
{
    int c;
    while (state->running)
    {
        c = getch_timeout(100);
        if (c == -1)
        {
            continue;
        }

        state->emu_mutex.lock();
        write_int_to_emu(state, TERM_IN, c);
        state->term_in_interrupt = true;
        state->emu_mutex.unlock();
        
        while (state->term_in_interrupt & state->running);
    }
}
