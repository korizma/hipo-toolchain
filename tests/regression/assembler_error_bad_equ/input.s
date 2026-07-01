.section text
a:
.word 0

.section data
b:
.word 0

.equ bad, a + b
.section out
.word bad
.end
