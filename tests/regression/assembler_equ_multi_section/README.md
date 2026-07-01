# assembler_equ_multi_section

Purpose: verifies that `.equ` can simplify multiple symbol differences from multiple sections when all section-relative terms cancel out.

Expected result: assembler succeeds and writes `15` (`0F 00 00 00`) to the `out` section.
