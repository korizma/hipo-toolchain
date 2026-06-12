# Current Project Progress Report

Date: 2026-06-12

Scope: status after the current local fixes. This report was updated only as documentation; no source code was changed for this report update.

## Executive Summary

The original `run.sh` crash path has been fixed enough that `tests/nivo-b/isr_terminal.s` now assembles without a segmentation fault. The instruction relocation paths in `src/asm/handlers_instructions.c` no longer dereference `sym->section->name` unconditionally; they now use `-1` when the referenced local symbol does not yet have a section.

That prevents the immediate crash, but it is not a complete relocation fix. In the current `nivo-b/isr_terminal.s` run, the relocation for the forward local symbol `message_start` is emitted with symbol index `-1` and remains that way in the printed relocation table. The symbol is completed later, but the earlier relocation entry is not repaired.

The literal-width work is also only partial. Some AST/storage fields now use `long`, but lexer/helper functions still return or accept `int`, so high 32-bit hex literals such as `0xFFFFFF04` still arrive as negative values like `-252`.

## Verification Performed

I built and ran the current tree from `/tmp` so no generated output files were written into the repository:

```sh
gcc /home/korizma/Documents/Faks/SS/Projekat/ss-projekat/src/main.c \
    /home/korizma/Documents/Faks/SS/Projekat/ss-projekat/src/grammar/*.c \
    /home/korizma/Documents/Faks/SS/Projekat/ss-projekat/src/asm/*.c \
    -o /tmp/ss-progress-check

/tmp/ss-progress-check \
    /home/korizma/Documents/Faks/SS/Projekat/ss-projekat/tests/nivo-b/isr_terminal.s
```

Result:

- Compile succeeded.
- Parse succeeded.
- The process exited with code `0`.
- The previous segmentation fault did not reproduce.
- The parsed high-address operands still printed as negative values:

```text
ld -252, %r1
st %r2, -256
```

The current relocation output includes this entry:

```text
Offset   Sym      Type           Addend
0x000016 -1       R_HIPO_12      0
```

That relocation corresponds to:

```asm
ld $message_start, %r1
```

`message_start` is later present in the final symbol table as a complete local symbol in section `isr`, but the relocation emitted before the label was defined still has `Sym == -1`.

## Fixed Since The Previous Report

The five instruction relocation sites that previously dereferenced `sym->section->name` now guard the section lookup:

- `src/asm/handlers_instructions.c:451`
- `src/asm/handlers_instructions.c:493`
- `src/asm/handlers_instructions.c:554`
- `src/asm/handlers_instructions.c:628`
- `src/asm/handlers_instructions.c:691`

Current behavior at those sites:

```c
int section_indx_in_sym_table = -1;
if (sym->section != 0)
{
    section_indx_in_sym_table = check_symbol_table(sym->section->name);
}
```

This removes the immediate null pointer dereference for forward local symbols in `ld` and `st` instruction operands.

Other relevant current changes visible in the tree:

- `src/asm/code.h` now stores expression and operand literals as `long`.
- `src/grammar/grammar.y` and the generated `src/grammar/grammar.tab.h` currently use `long literal` in `YYSTYPE`.
- `src/asm/handlers_instructions.c` now uses `long_fit_in_12b()` for the register-indirect literal load check.

## Remaining Relocation Issues

### 1. Instruction relocations can contain `Sym == -1`

The new guard prevents a crash but stores `-1` as the relocation symbol index when a local symbol is still unresolved. That is visible in the current `isr_terminal.s` output:

```text
0x000016 -1       R_HIPO_12      0
```

This is safer than crashing, but it is still not a valid final relocation unless a later pass intentionally resolves `-1` entries. The current printed output shows no such repair for `message_start`.

### 2. `.word symbol` still has the old crash pattern

`src/asm/handlers_directives.c:110` still does:

```c
int section_indx_in_sym_table = check_symbol_table(sym->section->name);
```

A forward local symbol used in `.word` can still reach this path with `sym->section == 0` and crash.

Example risk pattern:

```asm
.section test
.word later
later:
.word 0
```

### 3. There is still no final local-symbol relocation pass

The assembler needs a later pass that revisits relocations created before local symbols are complete. That pass should replace placeholder data with either:

- the final symbol index, if relocation should target the symbol directly, or
- the defining section symbol plus the symbol value as addend, if relocation should target the section.

Without that, the guard only turns the crash into an invalid or incomplete relocation record.

## Remaining Literal Issues

### 1. Helper functions still narrow literals through `int`

Even though several stored literal fields are now `long`, these helpers are still `int`-based:

- `src/grammar/helper.h:23` - `s_expr* expr_literal(int value);`
- `src/grammar/helper.h:38` - `s_sym_or_lit* new_lit_sl(int lit);`
- `src/grammar/helper.h:44` - `int parse_literal_lex(char* lit);`
- `src/grammar/helper.c:11` - `expr_literal(int value)`
- `src/grammar/helper.c:111` - `new_lit_sl(int lit)`
- `src/grammar/helper.c:150` - `parse_literal_lex()` returns `int`

The hex parsing still casts through `int`:

```c
return (int)strtol(lit+2, NULL, 16);
```

So values such as `0xFFFFFF04` still become negative on the current platform before reaching the wider `long` fields.

### 2. Explicit negative operands are still not part of the normal operand grammar

The lexer returns `MINUS` separately, and unary minus exists only in `expr`, which is used by `.equ`. Normal jump/load/store operands still accept `LITERAL` directly and do not accept `MINUS LITERAL`.

These forms are still not supported by the ordinary instruction/directive operand grammar:

```asm
ld $-4, %r1
ld [%r1 + -1], %r2
jmp -8
.word -1
```

### 3. Memory-mapped address handling is still implicit

`0xFFFFFF04` and `0xFFFFFF00` currently work only because they convert into negative signed values before encoding:

```text
0xFFFFFF04 -> -252
0xFFFFFF00 -> -256
```

If these are intended as valid 32-bit memory-mapped addresses, the assembler should parse them as an explicit 32-bit quantity and then apply the ISA's encoding rules deliberately.

## Error Handling Status

`src/asm/asm_main.c:21-27` still ignores the return value from `handle_line()`:

```c
for (int i = 0; i < asm_file.curr_line; i++)
{
    handle_line(asm_file.lines[i]);
}
```

That means handler errors can be printed but assembly continues anyway. This can leave partially generated sections, relocations, or symbol state in the final output.

## Recommended Next Fixes

1. Add the same unresolved-section guard to `.word symbol` in `handle_word()`, or route `.word` relocations through the same deferred relocation mechanism as instruction operands.

2. Add a final relocation-resolution pass after all labels and `.global` directives have been processed. This pass should repair placeholder entries such as `Sym == -1`.

3. Convert literal parsing helpers from `int` to `long` or a fixed-width target type consistently, including `parse_literal_lex()`, `new_lit_sl()`, and `expr_literal()`.

4. Decide whether signed literals are legal syntax in instruction operands and `.word`. If yes, add grammar support for `MINUS LITERAL` or a shared literal expression rule. If no, reject high hex values and signed-looking values deliberately instead of relying on overflow behavior.

5. Make `assemble_file()` stop or report failure when `handle_line()` returns `-1`.

## Short Status

The previous `isr_terminal.s` segmentation fault is fixed for the instruction relocation path, and the sample now exits normally. The assembler still emits an unresolved `-1` relocation for a forward local symbol, `.word` still has a similar null-dereference risk, and literal parsing still narrows through `int`.
