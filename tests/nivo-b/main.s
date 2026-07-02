# file: main.s

.global my_start, my_counter

.section my_code
my_start:
    ld $0xFFFFFEFE, %sp
    ld $20, %r3

print_message:
    ld $nivo_b_message, %r1
    ld $5, %r2

print_char:
    ld [%r1], %r4
    st %r4, 0xFFFFFF00 # term_out
    ld $1, %r5
    add %r5, %r1
    sub %r5, %r2
    bne %r2, %r0, print_char

    sub %r5, %r3
    bne %r3, %r0, print_message
    halt

nivo_b_message:
.ascii "press"

.section my_data
my_counter:
.word 0

.end
