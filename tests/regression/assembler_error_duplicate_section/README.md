# assembler_error_duplicate_section

Purpose: verifies that declaring the same section twice in one assembly file is reported as a duplicate symbol/section error.

Expected result: assembler exits with failure and prints `Double symbol declaration: text`.
