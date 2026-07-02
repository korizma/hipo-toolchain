.global start

.section text
start:
    ld $1, %r1
    ld $2, %r2
    ld $0, %r3

    beq %r1, %r2, fail
    ld $1, %r3
    bne %r1, %r2, bne_ok
fail:
    ld $0x6AD, %r10
    halt

bne_ok:
    bgt %r2, %r1, bgt_ok
    ld $0x6AD, %r10
    halt

bgt_ok:
    ld $0x55, %r4
    halt

.end

