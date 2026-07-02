.global start

.section text
start:
    ld data_value, %r1
    ld $data_value, %r2
    st %r1, [%r2 + 4]
    ld [%r2 + 4], %r3
    halt

.section data
data_value:
    .word 0x13579BDF
scratch:
    .word 0

.end

