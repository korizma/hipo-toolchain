# Assembly to Binary Mapping

Each machine instruction is 4 bytes wide.

```text
byte 0: OC[3:0] MOD[3:0]
byte 1: A[3:0]  B[3:0]
byte 2: C[3:0]  D[11:8]
byte 3: D[7:0]
```

Fields:

```text
OC   operation code
MOD  instruction modifier
A    register field A
B    register field B
C    register field C
D    signed 12-bit displacement
```

Register indexes:

```text
r0  = 0
r1  = 1
...
r13 = 13
r14 = 14 = sp
r15 = 15 = pc
```

CSR indexes:

```text
status  = 0
handler = 1
cause   = 2
```

For fields that are unused by an instruction, emit `0`.

## Basic Instructions

### `halt`

```text
OC=0x0 MOD=0x0 A=0 B=0 C=0 D=0
```

Effect:

```text
stop processor
```

### `int`

```text
OC=0x1 MOD=0x0 A=0 B=0 C=0 D=0
```

Effect:

```text
push status
push pc
cause <= 4
status <= status & ~0x1
pc <= handler
```

## Calls

### `call operand`, direct form

Use when `operand` can be represented as `gpr[A] + gpr[B] + D`.

Common assembler choice for PC-relative call:

```text
OC=0x2 MOD=0x0 A=pc B=r0 C=0 D=operand-pc
```

Effect:

```text
push pc
pc <= gpr[A] + gpr[B] + D
```

### `call operand`, indirect form

Use when the final destination is stored in memory, usually in a literal pool.

Common assembler choice:

```text
OC=0x2 MOD=0x1 A=pc B=r0 C=0 D=pool_entry-pc
```

Effect:

```text
push pc
pc <= mem32[gpr[A] + gpr[B] + D]
```

## Jumps

### `jmp operand`, direct form

Common assembler choice for PC-relative jump:

```text
OC=0x3 MOD=0x0 A=pc B=0 C=0 D=operand-pc
```

Effect:

```text
pc <= gpr[A] + D
```

### `beq %gpr1, %gpr2, operand`, direct form

```text
OC=0x3 MOD=0x1 A=pc B=gpr1 C=gpr2 D=operand-pc
```

Effect:

```text
if (gpr[B] == gpr[C]) pc <= gpr[A] + D
```

### `bne %gpr1, %gpr2, operand`, direct form

```text
OC=0x3 MOD=0x2 A=pc B=gpr1 C=gpr2 D=operand-pc
```

Effect:

```text
if (gpr[B] != gpr[C]) pc <= gpr[A] + D
```

### `bgt %gpr1, %gpr2, operand`, direct form

```text
OC=0x3 MOD=0x3 A=pc B=gpr1 C=gpr2 D=operand-pc
```

Effect:

```text
if (gpr[B] signed> gpr[C]) pc <= gpr[A] + D
```

### `jmp operand`, indirect form

Common assembler choice:

```text
OC=0x3 MOD=0x8 A=pc B=0 C=0 D=pool_entry-pc
```

Effect:

```text
pc <= mem32[gpr[A] + D]
```

### `beq %gpr1, %gpr2, operand`, indirect form

```text
OC=0x3 MOD=0x9 A=pc B=gpr1 C=gpr2 D=pool_entry-pc
```

Effect:

```text
if (gpr[B] == gpr[C]) pc <= mem32[gpr[A] + D]
```

### `bne %gpr1, %gpr2, operand`, indirect form

```text
OC=0x3 MOD=0xA A=pc B=gpr1 C=gpr2 D=pool_entry-pc
```

Effect:

```text
if (gpr[B] != gpr[C]) pc <= mem32[gpr[A] + D]
```

### `bgt %gpr1, %gpr2, operand`, indirect form

```text
OC=0x3 MOD=0xB A=pc B=gpr1 C=gpr2 D=pool_entry-pc
```

Effect:

```text
if (gpr[B] signed> gpr[C]) pc <= mem32[gpr[A] + D]
```

## Register Operations

### `xchg %gprS, %gprD`

```text
OC=0x4 MOD=0x0 A=0 B=gprD C=gprS D=0
```

Effect:

```text
temp <= gpr[B]
gpr[B] <= gpr[C]
gpr[C] <= temp
```

### `add %gprS, %gprD`

```text
OC=0x5 MOD=0x0 A=gprD B=gprD C=gprS D=0
```

Effect:

```text
gpr[A] <= gpr[B] + gpr[C]
```

### `sub %gprS, %gprD`

```text
OC=0x5 MOD=0x1 A=gprD B=gprD C=gprS D=0
```

Effect:

```text
gpr[A] <= gpr[B] - gpr[C]
```

### `mul %gprS, %gprD`

```text
OC=0x5 MOD=0x2 A=gprD B=gprD C=gprS D=0
```

Effect:

```text
gpr[A] <= gpr[B] * gpr[C]
```

### `div %gprS, %gprD`

```text
OC=0x5 MOD=0x3 A=gprD B=gprD C=gprS D=0
```

Effect:

```text
gpr[A] <= gpr[B] / gpr[C]
```

### `not %gpr`

```text
OC=0x6 MOD=0x0 A=gpr B=gpr C=0 D=0
```

Effect:

```text
gpr[A] <= ~gpr[B]
```

### `and %gprS, %gprD`

```text
OC=0x6 MOD=0x1 A=gprD B=gprD C=gprS D=0
```

Effect:

```text
gpr[A] <= gpr[B] & gpr[C]
```

### `or %gprS, %gprD`

```text
OC=0x6 MOD=0x2 A=gprD B=gprD C=gprS D=0
```

Effect:

```text
gpr[A] <= gpr[B] | gpr[C]
```

### `xor %gprS, %gprD`

```text
OC=0x6 MOD=0x3 A=gprD B=gprD C=gprS D=0
```

Effect:

```text
gpr[A] <= gpr[B] ^ gpr[C]
```

### `shl %gprS, %gprD`

```text
OC=0x7 MOD=0x0 A=gprD B=gprD C=gprS D=0
```

Effect:

```text
gpr[A] <= gpr[B] << gpr[C]
```

### `shr %gprS, %gprD`

```text
OC=0x7 MOD=0x1 A=gprD B=gprD C=gprS D=0
```

Effect:

```text
gpr[A] <= gpr[B] >> gpr[C]
```

## Store Instructions

### `st %gpr, [%reg + D]`

Also covers plain memory addressing after the assembler chooses registers/literals.

```text
OC=0x8 MOD=0x0 A=base_reg B=index_reg C=gpr D=disp
```

Effect:

```text
mem32[gpr[A] + gpr[B] + D] <= gpr[C]
```

For no index register, use `B=r0`.

### `st %gpr, mem32[%reg + D]`

Indirect store through a memory-held address.

```text
OC=0x8 MOD=0x2 A=base_reg B=index_reg C=gpr D=disp
```

Effect:

```text
mem32[mem32[gpr[A] + gpr[B] + D]] <= gpr[C]
```

### `push %gpr`

Pseudo-instruction encoded as pre-decrement store.

```text
OC=0x8 MOD=0x1 A=sp B=0 C=gpr D=-4
```

Effect:

```text
gpr[A] <= gpr[A] + D
mem32[gpr[A]] <= gpr[C]
```

With `A=sp` and `D=-4`:

```text
sp <= sp - 4
mem32[sp] <= gpr
```

## Load Instructions

### `csrrd %csr, %gpr`

```text
OC=0x9 MOD=0x0 A=gpr B=csr C=0 D=0
```

Effect:

```text
gpr[A] <= csr[B]
```

### `ld $literal_or_symbol, %gpr`

Immediate load. The value must be encodable through `D`, or assembled through an extra sequence/literal pool.

```text
OC=0x9 MOD=0x1 A=gpr B=r0 C=0 D=value
```

Effect:

```text
gpr[A] <= gpr[B] + D
```

With `B=r0`:

```text
gpr[A] <= D
```

### `ld %reg, %gpr`

Register move.

```text
OC=0x9 MOD=0x1 A=gpr B=reg C=0 D=0
```

Effect:

```text
gpr[A] <= gpr[B]
```

### `ld [%base + D], %gpr`

```text
OC=0x9 MOD=0x2 A=gpr B=base C=index D=disp
```

Effect:

```text
gpr[A] <= mem32[gpr[B] + gpr[C] + D]
```

For no index register, use `C=r0`.

### `pop %gpr`

Pseudo-instruction encoded as post-increment load.

```text
OC=0x9 MOD=0x3 A=gpr B=sp C=0 D=4
```

Effect:

```text
gpr[A] <= mem32[gpr[B]]
gpr[B] <= gpr[B] + D
```

With `B=sp` and `D=4`:

```text
gpr <= mem32[sp]
sp <= sp + 4
```

### `csrwr %gpr, %csr`

```text
OC=0x9 MOD=0x4 A=csr B=gpr C=0 D=0
```

Effect:

```text
csr[A] <= gpr[B]
```

### CSR immediate-or form

This is not directly exposed as a required assembly mnemonic, but the CPU supports it.

```text
OC=0x9 MOD=0x5 A=csr_dst B=csr_src C=0 D=disp
```

Effect:

```text
csr[A] <= csr[B] | D
```

### Load CSR from memory

This is not directly exposed as a required assembly mnemonic, but the CPU supports it.

```text
OC=0x9 MOD=0x6 A=csr B=base C=index D=disp
```

Effect:

```text
csr[A] <= mem32[gpr[B] + gpr[C] + D]
```

### Pop CSR from stack

Used by `iret`.

```text
OC=0x9 MOD=0x7 A=csr B=sp C=0 D=4
```

Effect:

```text
csr[A] <= mem32[gpr[B]]
gpr[B] <= gpr[B] + D
```

## Pseudo-Instructions

### `ret`

`ret` is encoded as `pop pc`.

```text
OC=0x9 MOD=0x3 A=pc B=sp C=0 D=4
```

Effect:

```text
pc <= mem32[sp]
sp <= sp + 4
```

### `iret`

`iret` expands into two load-family instructions.

First instruction, `pop pc`:

```text
OC=0x9 MOD=0x3 A=pc B=sp C=0 D=4
```

Second instruction, `pop status`:

```text
OC=0x9 MOD=0x7 A=status B=sp C=0 D=4
```

Effect:

```text
pc <= mem32[sp]
sp <= sp + 4
status <= mem32[sp]
sp <= sp + 4
```

