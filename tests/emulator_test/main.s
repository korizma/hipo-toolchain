.global start

.section text
start:
    # Stack setup for push/pop and call/ret checks.
    ld $0x700, %sp

    # Immediate loads and arithmetic.
    ld $5, %r1
    ld $7, %r2
    add %r2, %r1
    ld $3, %r3
    mul %r3, %r1
    ld $6, %r3
    div %r3, %r1
    ld $10, %r3
    sub %r1, %r3

    # Logic and shifts.
    ld $0x0F0, %r4
    ld $0x033, %r5
    and %r4, %r5
    ld $0x005, %r6
    or %r6, %r5
    ld $0x011, %r6
    xor %r6, %r5
    not %r6
    ld $1, %r7
    shl %r7, %r3
    shr %r7, %r3

    # Register exchange.
    xchg %r1, %r2

    # Absolute and register-indirect memory access.
    ld $0x234, %r8
    st %r8, 0x100
    ld 0x100, %r9

    ld $0x104, %r10
    ld $0x345, %r11
    st %r11, [%r10]
    ld [%r10], %r12

    # Stack LIFO behavior.
    push %r9
    push %r12
    pop %r13
    pop %r10

    # Conditional branch plus call/ret.
    ld $0, %r11
    beq %r10, %r9, branch_ok
    ld $0x6AD, %r11

branch_ok:
    call add_three
    jmp done

add_three:
    ld $3, %r7
    add %r7, %r11
    ret

done:
    ld $0x55, %r7
    halt

.end
