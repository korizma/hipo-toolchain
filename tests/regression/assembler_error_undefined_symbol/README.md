# assembler_error_undefined_symbol

Purpose: verifies that the assembler rejects a referenced symbol that is never defined or declared external.

Expected result: assembler exits with failure, prints `Undefined symbol: missing`, and does not write an object file.
