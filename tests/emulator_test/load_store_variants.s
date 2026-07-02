.global start

.section text
start:
    ld $0x700, %sp
    ld $0x55, %r1
    st %r1, 0x500
    ld 0x500, %r2

    ld $0x504, %r3
    ld $0x123, %r4
    st %r4, [%r3]
    ld [%r3], %r5

    st %r2, [%r3 + 4]
    ld [%r3 + 4], %r6
    halt

.end
