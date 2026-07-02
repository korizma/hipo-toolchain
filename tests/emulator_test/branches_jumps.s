.global start

.section text
start:
    ld $noop_interrupt_handler, %r1
    csrwr %r1, %handler

    ld $3, %r1
    ld $3, %r2
    beq %r1, %r2, beq_ok
    ld $0x6AD, %r10

beq_ok:
    ld $5, %r3
    bne %r1, %r3, bne_ok
    ld $0x6AD, %r10

bne_ok:
    bgt %r3, %r1, bgt_ok
    ld $0x6AD, %r10

bgt_ok:
    jmp done
    ld $0x6AD, %r10

done:
    ld $0x55, %r4
    halt

noop_interrupt_handler:
    iret

.end
