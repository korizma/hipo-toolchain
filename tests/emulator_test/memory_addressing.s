.global start

.section text
start:
    ld $noop_interrupt_handler, %r1
    csrwr %r1, %handler

    ld $0x123, %r1
    st %r1, 0x300
    ld 0x300, %r2

    ld $0x340, %r3
    ld $0x456, %r4
    st %r4, [%r3]
    ld [%r3], %r5

    ld $0x789, %r6
    st %r6, [%r3 + 4]
    ld [%r3 + 4], %r7

    ld $data_word, %r8
    ld data_word, %r9
    st %r2, data_word
    ld data_word, %r10
    halt

noop_interrupt_handler:
    iret

data_word:
    .word 0x55

.end
