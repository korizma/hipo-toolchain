.global start

.section text
start:
    ld $0x700, %sp
    ld $0x111, %r1
    ld $0x222, %r2
    push %r1
    push %r2
    pop %r3
    pop %r4

    ld $5, %r5
    call add_two
    ld $0x55, %r7
    halt

add_two:
    ld $2, %r6
    add %r6, %r5
    ret

.end
