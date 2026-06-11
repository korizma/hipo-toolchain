# Opis asemblera

Sve instrukcije koje su dozvoljene su date ovde:

```text
halt                         Zaustavlja izvršavanje instrukcija                 A
int                          Izaziva softverski prekid                          A
iret                         pop pc; pop status;                                A
call operand                 push pc; pc <= operand;                            A
ret                          pop pc;                                            A
jmp operand                  pc <= operand;                                     A
beq %gpr1, %gpr2, operand    if (gpr1 == gpr2) pc <= operand;                   A
bne %gpr1, %gpr2, operand    if (gpr1 != gpr2) pc <= operand;                   A
bgt %gpr1, %gpr2, operand    if (gpr1 signed> gpr2) pc <= operand;              A
push %gpr                    sp <= sp - 4; mem32[sp] <= gpr;                    A
pop %gpr                     gpr <= mem32[sp]; sp <= sp + 4;                    A
xchg %gprS, %gprD            temp <= gprD; gprD <= gprS; gprS <= temp;          A
add %gprS, %gprD             gprD <= gprD + gprS;                               A
sub %gprS, %gprD             gprD <= gprD - gprS;                               A
mul %gprS, %gprD             gprD <= gprD * gprS;                               A
div %gprS, %gprD             gprD <= gprD / gprS;                               A
not %gpr                     gpr <= ~gpr;                                       A
and %gprS, %gprD             gprD <= gprD & gprS;                               A
or %gprS, %gprD              gprD <= gprD | gprS                                A
xor %gprS, %gprD             gprD <= gprD ^ gprS;                               A
shl %gprS, %gprD             gprD <= gprD << gprS;                              A
shr %gprS, %gprD             gprD <= gprD >> gprS;                              A
ld operand, %gpr             gpr <= operand;                                    A
st %gpr, operand             operand <= gpr;                                    A
csrrd %csr, %gpr             gpr <= csr                                         A
csrwr %gpr, %csr             csr <= gpr;                                        A
```

Sve asemblerske direktive su date ovde:
```text
.extern <lista-simbola>
.global <lista-simbola>
.section <ime-sekcije>
.word <lista-simbola ili literala>
.skip <broj-bajtova>
.ascii <string>
.equ <novi_simobl>, <izraz>
.end
```

Operandi u instrukcijama za rad sa podacima
```
$<literal> - vrednost <literal>
$<simbol> - vrednost <simbol>
<literal> - vrednost iz memorije na adresi <literal>
<simbol> - vrednost iz memorije na adresi <simbol>
%<reg> - vrednost u registru <reg>
[%<reg>] - vrednost iz memorije na adresi <reg>
[%<reg> + <literal>] - vrednost iz memorije na adresi <reg> + <literal>1
[%<reg> + <simbol>] - vrednost iz memorije na adresi <reg> + <simbol>2
```