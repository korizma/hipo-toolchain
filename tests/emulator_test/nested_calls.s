.global start

.section text
start:
    ld $0x700, %sp
    ld $1, %r1
    call first
    ld %sp, %r4
    halt

first:
    ld $2, %r2
    add %r2, %r1
    call second
    ret

second:
    ld $4, %r3
    add %r3, %r1
    ret

.end

