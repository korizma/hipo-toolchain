# TODO

Generated from a repository scan on 2026-06-29.

## Build blockers

- [x] Fix the active build entrypoint. `make all` fails because `src/main.c:3` and `src/main.c:6` include `asm/code.h` and `asm/asm_main.h`, but the repo has no `src/asm` directory. Decide whether the parser should target the newer C++ code in `src/c_src` or restore the old C assembler API.
- [x] Update parser helper includes and data model. `src/grammar/helper.c:1`, `src/grammar/helper.c:6`, `src/grammar/helper.h:4`, and `src/grammar/grammar.y` still depend on the missing `src/asm` headers and `s_asm_file`.
- [x] Fix `run.sh:3`; it compiles `src/asm/*.c`, which no longer exists.
- [x] Add forward declarations for `s_section` and `s_asm_instruction` before they are used in `src/c_src/symbol_table.hpp:25`. The current C++ syntax pass fails on `s_mem_reg_sym`.
- [x] Remove the duplicate default argument from `src/c_src/error.cpp:3`; the default already belongs in `src/c_src/error.hpp:29`.
- [x] Add helper prototypes before use, or move definitions above callers:
  - `src/c_src/asm_instruction_call_branch.cpp:14` calls `_handle_jump_branch` before it is declared.
  - `src/c_src/asm_instruction_store.cpp:17` through `src/c_src/asm_instruction_store.cpp:38` call `_handle_st_*` helpers before declarations.
- [x] Fix `add_mem_reg_sym_to_symbol` call sites in `src/c_src/asm_instruction_load.cpp:274` and `src/c_src/asm_instruction_store.cpp:204`; the function now takes the original `s_asm_instruction*` as a fourth argument.

## Real stubs and placeholders

- [ ] Implement or remove `import_section` in `src/c_src/section.cpp:71`. The header marks the signature as not final, and the function currently returns `nullptr` as a placeholder.
- [ ] Implement or remove `import_rela_table` in `src/c_src/rela_table.cpp:47`. The header marks the signature as not final, and the function returns `NULL`.
- [ ] Implement `rela_table_to_string` in `src/c_src/rela_table.cpp:42`; it currently returns an empty string, so generated object output loses relocation tables.
- [ ] Implement or remove `import_symbol_table`, declared in `src/c_src/symbol_table.hpp:77`. There is no definition.
- [ ] Implement `symbol_table_entry_to_bytes`, declared in `src/c_src/symbol_table.hpp:71` and repeated as a declaration in `src/c_src/symbol_table.cpp:73`.
- [x] Implement `asm_line_to_string`, declared in `src/c_src/asm.hpp:65` and used by `src/c_src/asm.cpp:158`.
- [x] Implement `add_section_to_program`, declared in `src/c_src/asm.hpp:78` and used by `src/c_src/asm_directive.cpp:100`.
- [ ] Implement `asm_instruction_to_string` in `src/c_src/asm_instruction.cpp:108`; it currently returns an empty string.
- [ ] Implement `asm_directive_to_string` in `src/c_src/asm_directive.cpp:184`; it currently returns an empty string.
- [ ] Implement EQU memory-register symbol handling in `src/c_src/symbol_table.cpp:163`. The comments at `src/c_src/symbol_table.cpp:170` and `src/c_src/symbol_table.cpp:171` describe the intended relocation/instruction rewrite behavior, but the function currently returns no errors and performs no work.
- [ ] Finish symbol trampoline handling in `src/c_src/trampoline.cpp:53`. The `TRMP_SYM` branch now returns safely, but still only has a placeholder and does not emit or relocate symbol trampolines.
- [ ] Replace README placeholders for linker and emulator in `README.md:12` through `README.md:18`.

## Incorrect or incomplete behavior found while scanning

- [x] Fix error handling in `src/c_src/asm.cpp:94`. It currently treats `error.no_error == true` as an error, which is inverted.
- [x] Make `assemble_program_to_file` in `src/c_src/asm.cpp:63` stop and return `false` when `assemble_lines()` fails instead of always writing output.
- [x] Fix `get_symbol_entry_symbol` in `src/c_src/symbol_table.cpp:19` and `get_and_create_new_symbol_entry` in `src/c_src/symbol_table.cpp:53`; both return the address of a loop-local copy instead of the vector element.
- [x] Fix the loop in `_finalize_equ_symbols_symbol_table` in `src/c_src/symbol_table.cpp:130`; it iterates by value, so changes to symbol entries are lost.
- [ ] Fix the loop in `_check_equ_mem_reg_sym_addressing` in `src/c_src/symbol_table.cpp:166`; it still iterates by value, so changes to symbol entries would be lost once that placeholder is implemented.
- [ ] Add missing newline separators in `symbol_table_to_string` after each entry in `src/c_src/symbol_table.cpp:108`.
- [x] Fix `write_trampoline_entry` in `src/c_src/trampoline.cpp:53`; it uses assignment (`=`) instead of comparison (`==`) for `TRMP_SYM`.
- [x] Ensure all non-void functions return on every path:
  - `src/c_src/trampoline.cpp:58`
  - `src/c_src/asm_instruction_call_branch.cpp:104`
  - `src/c_src/asm_instruction_load.cpp:33`
  - `src/c_src/asm_instruction_store.cpp:41`
- [x] Fix binary opcode literals in load/store code. Many places use decimal `1000` or `1001` instead of binary `0b1000` or `0b1001`, causing `char` overflow warnings and wrong opcodes.
- [x] Initialize heap-allocated structs or switch to constructors. `new_section`, `new_asm_instruction`, `new_asm_directive`, `init_program`, and `new_rela_table` use `malloc` with C++ types such as `std::vector` and `std::string`, which bypasses constructors and leaves fields uninitialized.
- [ ] Create relocation tables before adding entries. `create_new_rela_table_entry` calls `get_rela_table(section)` and immediately writes to it; callers like `.word` do not guarantee `new_rela_table(section)` has already been called.

## Larger missing project areas

- [ ] Implement the linker executable and command-line interface described in `docs/postavka/linker.md`. The only linker-specific work in the repo is the plan in `docs/implementacija/linker_plan.md`.
- [ ] Implement linker `-relocatable` mode:
  - [ ] Merge symbol tables from multiple object files.
  - [ ] Detect duplicate symbol definitions, including section-name conflicts where required.
  - [ ] Merge same-named sections in input order.
  - [ ] Carry forward or resolve relocation records.
  - [ ] Resolve external symbols that become known through definitions or `equ`.
- [ ] Implement linker `-hex` mode:
  - [ ] Lay out sections in memory.
  - [ ] Support and validate `-place=<section>@<address>`.
  - [ ] Detect overlaps caused by placement.
  - [ ] Reject unresolved symbols.
  - [ ] Apply all relocations.
  - [ ] Emit the required address/content hex output.
- [ ] Implement the emulator executable and command-line interface described in `docs/postavka/emulator.md`. The README currently has only an emulator placeholder.
- [ ] Add end-to-end tests for assembler output, linker relocatable output, linker hex output, and emulator execution using `tests/nivo-a`, `tests/nivo-b`, and `tests/nivo-c`.

## Verification commands used

- `rg -n -i "todo|fixme|xxx|hack|stub|placeholder|not implemented|unimplemented|implementirati|nedostaje|treba|tbd|dummy|mock|privremeno|za sad|zasad|plan|future|later|return null|return nullptr|throw|abort|assert\\(false" src docs README.md Makefile run.sh tests`
- `make all`
- `make build-asm`
- `bin/assembler -o /tmp/ss-parser-smoke2.bin tests/nivo-a/isr_software.s`
- `bin/assembler -o /tmp/ss-parser-main.bin tests/nivo-a/main.s`
- `bin/assembler -o /tmp/ss-parser-math.bin tests/nivo-a/math.s`
- `g++ -std=c++17 -Wall -Wextra -fsyntax-only src/c_src/*.cpp`
