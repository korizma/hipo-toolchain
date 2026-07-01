# assembler_relocation_local

Purpose: verifies that a `.word` reference to a local symbol is represented as a relocation against its section symbol.

Expected result: assembler succeeds and the relocation entry points at symbol table index `0`, the `text` section symbol.
