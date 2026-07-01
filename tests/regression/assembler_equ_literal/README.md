# assembler_equ_literal

Purpose: verifies that a literal-only `.equ` expression is calculated and emitted through `.word`.

Expected result: assembler succeeds and the `data` section contains `0A 00 00 00`.
