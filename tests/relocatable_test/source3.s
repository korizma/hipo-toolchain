.extern podatak1, podatak2

.global podatak3

.section glavni
podatak3:
.ascii "3333"
.word podatak1, podatak2, podatak3

.section filler
.word 0x3, 0x3, 0x3, 0x3

.end
