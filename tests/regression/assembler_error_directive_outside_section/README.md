# assembler_error_directive_outside_section

Purpose: verifies that section-dependent directives are rejected before any `.section` directive opens a section.

Expected result: assembler exits with failure and prints `Line not in section`.
