.section text
a:
.skip 4
b:
.skip 4

.section data
c:
.skip 8
d:
.skip 4

.equ delta, b - a + d - c + 3

.section out
.word delta
.end
