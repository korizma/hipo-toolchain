set -e

./assembler -o main.o tests/nivo-c/main.s 
./assembler -o handler.o tests/nivo-c/handler.s 
./assembler -o isr_terminal.o tests/nivo-c/isr_terminal.s 
./assembler -o isr_timer.o tests/nivo-c/isr_timer.s 

./linker -hex -place=code@0x40000000 -o program.hex main.o isr_terminal.o isr_timer.o handler.o
./emulator program.hex