# assembler_error_bad_equ

Purpose: verifies that the assembler rejects an `.equ` expression that cannot be reduced to an absolute value or one relocatable symbol.

Expected result: assembler exits with failure and prints `EQU expression is not calculatable: bad`.
