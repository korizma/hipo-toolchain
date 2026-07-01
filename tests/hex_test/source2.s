.extern podatak1, podatak3

.global podatak2

.section glavni
podatak2:
.ascii "2222"
.word podatak1, podatak2, podatak3

.section filler
.word 0x2, 0x2, 0x2, 0x2

.end
