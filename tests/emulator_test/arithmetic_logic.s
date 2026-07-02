.global start

.section text
start:
    ld $noop_interrupt_handler, %r1
    csrwr %r1, %handler

    ld $9, %r1
    ld $4, %r2
    add %r2, %r1
    ld $20, %r3
    sub %r2, %r3
    mul %r2, %r1
    ld $13, %r4
    div %r2, %r4

    ld $0x0F0, %r5
    ld $0x033, %r6
    and %r5, %r6
    ld $0x005, %r7
    or %r7, %r6
    ld $0x011, %r8
    xor %r8, %r6
    not %r8

    ld $2, %r9
    ld $3, %r10
    shl %r9, %r10
    shr %r9, %r10

    xchg %r1, %r4
    halt

noop_interrupt_handler:
    iret

.end
