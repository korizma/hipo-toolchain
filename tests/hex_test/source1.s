.extern podatak2, podatak3

.global podatak1

.section glavni
podatak1:
.ascii "1111"
.word podatak1, podatak2, podatak3

.section filler
.word 0x1, 0x1, 0x1, 0x1

.end
