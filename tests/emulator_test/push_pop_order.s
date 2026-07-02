.global start

.section text
start:
    ld $noop_interrupt_handler, %r1
    csrwr %r1, %handler

    ld $0x700, %sp
    ld $0x10, %r1
    ld $0x20, %r2
    ld $0x30, %r3
    push %r1
    push %r2
    push %r3
    pop %r4
    pop %r5
    pop %r6
    ld %sp, %r7
    halt

noop_interrupt_handler:
    iret

.end
