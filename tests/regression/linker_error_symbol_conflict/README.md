# linker_error_symbol_conflict

Purpose: verifies that relocatable linking rejects two object files that both define the same non-section global symbol.

Expected result: linker exits with failure and prints `Symbol conflict: duplicate`.
