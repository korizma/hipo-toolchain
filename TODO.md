# TODO

Current active work only. Completed historical items have been removed.

## Build Blockers

- [ ] Fix `src/c_src/rela_table.hpp:45` and `src/c_src/rela_table.hpp:48` so `s_symbol_table_entry` is declared before use. A read-only syntax check currently fails when compiling `src/c_src/rela_table.cpp` directly.
- [ ] Decide whether the built assembler should be named `bin/assembler` or `asembler` as specified in `docs/postavka/assembler.md`; align the `Makefile` and docs before submission.

## Assembler Correctness

- [ ] Fix `push %gpr` code generation in `src/c_src/asm_instruction.cpp:191`; the store instruction currently writes `%r0` because `reg_c` is set to `0` instead of `instruction->reg1`.
- [ ] Fix `iret` code generation in `src/c_src/asm_instruction.cpp:142`; the current sequence pops the top stack word into `status` and the next word into `pc`, while interrupt frames are pushed as `status` then `pc`.
- [ ] Fix `sub` and `div` operand order in `src/c_src/asm_instruction.cpp:273` and `src/c_src/asm_instruction.cpp:307`; generated machine instructions compute `source op destination` instead of `destination op source`.
- [ ] Fix `ld [%reg + literal], %dst` and `ld [%reg + symbol], %dst` in `src/c_src/asm_instruction_load.cpp:239` and `src/c_src/asm_instruction_load.cpp:266`; they currently emit store opcode/register fields instead of load-from-memory instructions.
- [ ] Finish symbol trampoline and relocation handling for symbolic jump/load/store operands:
  - [ ] Implement the `TRMP_SYM` branch in `src/c_src/trampoline.cpp:53` so symbol trampoline words are emitted, relocated, and instruction displacements are patched.
  - [ ] Replace symbol-index-as-literal trampolines in `src/c_src/asm_instruction_load.cpp:107`, `src/c_src/asm_instruction_load.cpp:188`, and `src/c_src/asm_instruction_store.cpp:86` with symbol-aware trampoline or relocation handling.
- [ ] Finish register-plus-symbol addressing validation and patching:
  - [ ] Store or use displacement byte offset `instruction_offset + 2` in `add_mem_reg_sym_to_symbol` / `_check_equ_mem_reg_sym_addressing`; the current stored offset points at the start of the instruction, not the 12-bit displacement field.
  - [ ] Check that final EQU literal values fit signed 12 bits before writing `[reg + sym]` displacements.
  - [ ] Keep rejecting non-EQU or non-literal symbols for `[reg + sym]`, as required by `docs/postavka/assembler.md`.
- [ ] Harden EQU finalization in `src/c_src/symbol_table.cpp:139`:
  - [ ] Report undefined symbols found by `equ_set_symbol_indexes` instead of dereferencing invalid indexes.
  - [ ] Use expression validity and section rules for coefficients instead of accepting any single remaining symbol blindly.
  - [ ] Add an `ERR_EQU_NOT_CALCULATABLE` message in `src/c_src/error.cpp:19`.

## Relocation and Object Import

- [ ] Implement relocation rewrite helpers in `src/c_src/rela_table.cpp:80` and `src/c_src/rela_table.cpp:85`.
- [ ] Wire EQU relocation updating into `finalize_symbol_table`; `_rela_equ_updating` exists but is not called.
- [x] Fix the assignment used as a condition in `_rela_equ_updating` at `src/c_src/symbol_table.cpp:188` (`=` vs `==`) before enabling that path.
- [ ] Implement object-file import/parsing helpers needed by the linker:
  - [ ] `import_section` in `src/c_src/section.cpp:75`.
  - [ ] `import_rela_table` in `src/c_src/rela_table.cpp:75`.
  - [ ] `import_symbol_table`, declared in `src/c_src/symbol_table.hpp:76`.
- [ ] Reconcile `docs/implementacija/assembled_format.md` with the current textual assembler output (`#.symtab`, section hex dumps, `#.rela.*`) or implement the binary format described there.

## Linker

- [ ] Replace the `src/ld_main.cpp:8` placeholder with the real CLI described in `docs/postavka/linker.md` (`-o`, `-hex`, `-relocatable`, `-place`).
- [ ] Implement linker object loading from assembler output.
- [ ] Implement `-relocatable` mode:
  - [ ] Merge symbol tables from all inputs.
  - [ ] Detect duplicate symbol definitions and section-name conflicts as required.
  - [ ] Merge same-named sections in input order.
  - [ ] Rebase section offsets and symbol values, and carry forward unresolved relocations.
  - [ ] Resolve extern/EQU symbols that become known.
- [ ] Implement `-hex` mode:
  - [ ] Lay out sections in memory, including same-named section aggregation.
  - [ ] Apply and validate `-place=<section>@<address>`.
  - [ ] Detect overlaps.
  - [ ] Reject unresolved symbols.
  - [ ] Apply all relocations.
  - [ ] Emit the required address/content hex output.

## Emulator

- [ ] Replace the `src/emu_main.cpp:8` placeholder with the real CLI described in `docs/postavka/emulator.md`.
- [ ] Load linker `-hex` memory initialization output.
- [ ] Implement CPU state, instruction fetch/decode/execute, and halt final register dump.
- [ ] Implement memory-mapped terminal and timer behavior from `docs/postavka/architecture.md`.

## Tests and Docs

- [ ] Add assembler regression tests for `push`, `iret`, `sub`, `div`, `ld [%reg + literal]`, `[reg + sym]`, symbol trampolines, `.word` relocations, and EQU edge cases.
- [ ] Add end-to-end tests for linker relocatable output, linker hex output, and emulator execution using `tests/nivo-a`, `tests/nivo-b`, and `tests/nivo-c`.
- [ ] Update `README.md`: fix the missing `docs/asm_desc.md` link and replace linker/emulator placeholders.
- [ ] Document generated parser-file policy for submission; `make clean` already removes flex/bison outputs.

## Verification Used

- `rg -n "TODO|FIXME|for later|i_dont_know_yet|return NULL|return nullptr|not implemented|\\.\\.\\.|asm_desc|= STB_|add_literal_trampoline_entry" README.md src docs/implementacija tests Makefile`
- `g++ -std=c++17 -Wall -Wextra -fsyntax-only src/c_src/*.cpp`
- Source comparison against `docs/postavka/assembler.md` and `docs/postavka/architecture.md`.
- `make all` was not run during this update because it regenerates parser files under `src/c_src`.
