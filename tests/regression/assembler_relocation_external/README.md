# assembler_relocation_external

Purpose: verifies that a `.word` reference to an external symbol emits a 32-bit relocation entry.

Expected result: assembler succeeds and the object file contains a `#.rela.text` table with an `R_HIPO_32` entry at offset `0`.
