# assembler_error_instruction_outside_section

Purpose: verifies that instructions are rejected before any `.section` directive opens a section.

Expected result: assembler exits with failure and prints `Line not in section`.
