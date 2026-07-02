.global start

.section text
start:
    ld $noop_interrupt_handler, %r1
    csrwr %r1, %handler

    ld data_value, %r1
    ld $data_value, %r2
    st %r1, [%r2 + 4]
    ld [%r2 + 4], %r3
    halt

noop_interrupt_handler:
    iret

.section data
data_value:
    .word 0x13579BDF
scratch:
    .word 0

.end
