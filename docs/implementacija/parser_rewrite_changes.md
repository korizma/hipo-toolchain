# Parser Rewrite Changes

Date: 2026-06-29

## Goal

Rewrite the assembler grammar so it no longer depends on the removed C assembler API from `src/asm`, and instead builds the new C++ assembler model in `src/c_src`.

## Parser And Helpers

- `src/grammar/grammar.y`
  - Removed old includes for `../asm/code.h` and `helper.h`.
  - Uses `helper.hpp` and the C++ assembler structs/enums.
  - Replaced old `new_empty_line()` and `add_line()` actions with helper calls that create directives, instructions, labels, expressions, and operands for the new program model.
  - Replaced old manual C array list handling with parser helper list types backed by `std::vector`.
  - Kept the accepted assembly syntax the same: labels, directives, jump operands, load/store operands, CSR operands, and expressions still parse in the same forms.

- `src/grammar/helper.hpp` and `src/grammar/helper.cpp`
  - Added parser-only helper structs for jump operands and load/store operands.
  - Added list builders for symbol lists and `.word` symbol/literal lists.
  - Added directive builder helpers that fill `s_asm_directive` and call `add_directive_to_program`.
  - Added instruction builder helpers that fill `s_asm_instruction` and call `add_instruction_to_program`.
  - Mapped CSR instructions correctly through `control_reg`, which is what the new instruction handlers expect.
  - Kept register and literal lexing helpers in the parser helper layer.

- `src/grammar/lexer.l`
  - Switched the helper include from `helper.h` to `helper.hpp`.

## Build And Entrypoint

- `Makefile`
  - Builds generated Bison/Flex files from `src/c_src`.
  - Compiles generated `.c` parser/lexer files as C++ because the grammar now uses C++ types.
  - Builds `src/grammar/helper.cpp` instead of the deleted C helper.
  - Includes dependency files for common C++ objects so header changes rebuild dependent files.

- `src/asm_main.cpp`
  - Uses `init_program()`, `yyparse()`, `assemble_program_to_file()`, and `free_program()`.
  - Removed old `s_asm_file`, `init_asm_file()`, and `assemble_file()` usage.

## Required C++ Backend Fixes

These were needed so the new parser could safely create and pass C++ objects into the assembler backend.

- Object construction
  - Switched parser-facing factory functions from `malloc` to C++ construction for structs containing `std::string` or `std::vector`.
  - Affected `new_asm_instruction`, `new_asm_directive`, `new_expression`, `new_section`, `new_rela_table`, and `init_program`.

- Program assembly flow
  - `assemble_program_to_file` now uses the already parsed program instead of reinitializing it.
  - It returns failure if `assemble_lines()` fails.
  - `assemble_lines()` now treats `error.no_error == false` as an error.
  - Added `add_section_to_program`.
  - Added a minimal `asm_line_to_string` for diagnostics.

- Directives
  - `.end` now returns a no-error result through directive handling.
  - `.word` now returns success after writing all operands.

- Symbol table
  - Fixed symbol lookup functions that returned addresses of loop-local copies.
  - `_finalize_equ_symbols_symbol_table` now iterates by reference so EQU changes are kept.

- Load/store and branch handling
  - Wired `handle_ld` to the existing load helper functions.
  - Fixed decimal opcode literals `1000` and `1001` to binary `0b1000` and `0b1001`.
  - Added missing returns in load/store, jump/branch, and trampoline paths.

- Trampolines and byte patching
  - Fixed `TRMP_SYM` comparison in `write_trampoline_entry`.
  - Made `write_to_lower_12b` take the section byte vector by reference so trampoline displacement patches affect real output.
  - Reserved program line and section vectors during initialization to avoid invalidating backend pointers that currently point into `program.section_list`.

## TODO Updates

Checked completed items in `TODO.md` for:

- Parser helper/data model migration.
- `asm_line_to_string`.
- `add_section_to_program`.
- Symbol lookup pointer fixes.
- `_finalize_equ_symbols_symbol_table` reference iteration.
- `write_trampoline_entry` assignment/comparison bug.
- Missing return paths listed in the TODO.
- Load/store binary opcode literals.
- C++ constructor initialization for vector/string-containing structs.

The TODO item for `_check_equ_mem_reg_sym_addressing` remains open because that function is still a placeholder. Symbol trampoline handling also remains open because the `TRMP_SYM` path now returns safely, but it still does not perform real symbol trampoline relocation.

## Verification

- `make build-asm`
- `make all`
- `bin/assembler -o /tmp/ss-parser-smoke2.bin tests/nivo-a/isr_software.s`
- `bin/assembler -o /tmp/ss-parser-main.bin tests/nivo-a/main.s`
- `bin/assembler -o /tmp/ss-parser-math.bin tests/nivo-a/math.s`

All provided `.s` files reached `Parse succeeded` during parser coverage checks. Some standalone assemblies still fail later because of unresolved symbols or unfinished backend semantics; those are not parser failures.
