.global start

.section text
start:
    ld $0x700, %sp
    ld $handler, %r1
    csrwr %r1, %handler
    ld $0x0FF, %r2
    csrwr %r2, %status
    csrrd %handler, %r3
    csrrd %status, %r4
    int
    ld $0x6AD, %r10

handler:
    ld $0x6AD, %r11
    csrrd %cause, %r5
    csrrd %status, %r6
    ld %sp, %r7
    halt

.end
