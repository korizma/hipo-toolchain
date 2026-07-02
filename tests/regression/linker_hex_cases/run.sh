#!/usr/bin/env sh
set -eu

TEST_DIR="$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)"
REPO_ROOT="$(CDPATH= cd -- "$TEST_DIR/../../.." && pwd)"
. "$TEST_DIR/../test_lib.sh"

prepare_test_bin "$TEST_DIR"

tmp="$(mktemp -d)"
trap 'rm -rf "$tmp"' EXIT

write_source()
{
    file="$1"
    shift
    printf '%s\n' "$@" >"$file"
}

assemble()
{
    name="$1"
    src="$2"
    out="$TEST_DIR/bin/$name.o"
    capture "$TEST_DIR/bin/asm_$name.log" "$REPO_ROOT/bin/assembler" -o "$out" "$src"
    assert_success "$CAPTURE_STATUS" "assembler $name"
}

link_hex()
{
    name="$1"
    shift
    capture "$TEST_DIR/bin/link_$name.log" "$REPO_ROOT/bin/linker" -hex -o "$TEST_DIR/bin/$name.hex" "$@"
    assert_success "$CAPTURE_STATUS" "linker $name"
    assert_file_exists "$TEST_DIR/bin/$name.hex"
}

expect_hex()
{
    file="$1"
    text="$2"
    assert_contains "$file" "$text"
}

# 1. Single section is placed at address zero by default.
write_source "$tmp/default_text.s" \
    ".section text" \
    ".word 0x11223344" \
    ".end"
assemble default_text "$tmp/default_text.s"
link_hex default_text "$TEST_DIR/bin/default_text.o"
expect_hex "$TEST_DIR/bin/default_text.hex" "00000000: 44 33 22 11"

# 2. Explicit placement moves the section to the requested address.
write_source "$tmp/placed_text.s" \
    ".section text" \
    ".word 0x01020304" \
    ".end"
assemble placed_text "$tmp/placed_text.s"
link_hex placed_text -place=text@0x10 "$TEST_DIR/bin/placed_text.o"
expect_hex "$TEST_DIR/bin/placed_text.hex" "00000010: 04 03 02 01"

# 3. Unplaced sections start after the highest explicitly placed section.
write_source "$tmp/auto_after_place.s" \
    ".section text" \
    ".word 0xAAAAAAAA" \
    ".section data" \
    ".word 0xBBBBBBBB" \
    ".end"
assemble auto_after_place "$tmp/auto_after_place.s"
link_hex auto_after_place -place=data@0x20 "$TEST_DIR/bin/auto_after_place.o"
expect_hex "$TEST_DIR/bin/auto_after_place.hex" "00000020: BB BB BB BB"
expect_hex "$TEST_DIR/bin/auto_after_place.hex" "00000024: AA AA AA AA"

# 4. Same-named sections from multiple inputs are concatenated.
write_source "$tmp/merge_left.s" \
    ".section text" \
    ".word 0x11111111" \
    ".end"
write_source "$tmp/merge_right.s" \
    ".section text" \
    ".word 0x22222222" \
    ".end"
assemble merge_left "$tmp/merge_left.s"
assemble merge_right "$tmp/merge_right.s"
link_hex merge_sections "$TEST_DIR/bin/merge_left.o" "$TEST_DIR/bin/merge_right.o"
expect_hex "$TEST_DIR/bin/merge_sections.hex" "00000000: 11 11 11 11 22 22 22 22"

# 5. Local symbol relocation writes the placed symbol address.
write_source "$tmp/local_reloc.s" \
    ".section text" \
    ".word target" \
    "target:" \
    ".word 0xDEADBEEF" \
    ".end"
assemble local_reloc "$tmp/local_reloc.s"
link_hex local_reloc -place=text@0x100 "$TEST_DIR/bin/local_reloc.o"
expect_hex "$TEST_DIR/bin/local_reloc.hex" "00000100: 04 01 00 00 EF BE AD DE"

# 6. External global relocation resolves across input files.
write_source "$tmp/extern_ref.s" \
    ".extern shared" \
    ".section text" \
    ".word shared" \
    ".end"
write_source "$tmp/global_def.s" \
    ".global shared" \
    ".section data" \
    "shared:" \
    ".word 0xCAFEBABE" \
    ".end"
assemble extern_ref "$tmp/extern_ref.s"
assemble global_def "$tmp/global_def.s"
link_hex extern_global -place=text@0x40 -place=data@0x80 "$TEST_DIR/bin/extern_ref.o" "$TEST_DIR/bin/global_def.o"
expect_hex "$TEST_DIR/bin/extern_global.hex" "00000040: 80 00 00 00"
expect_hex "$TEST_DIR/bin/extern_global.hex" "00000080: BE BA FE CA"

# 7. Non-aligned placement still emits the requested starting address.
write_source "$tmp/non_aligned.s" \
    ".section text" \
    ".word 0x01020304" \
    ".end"
assemble non_aligned "$tmp/non_aligned.s"
link_hex non_aligned -place=text@0x3 "$TEST_DIR/bin/non_aligned.o"
expect_hex "$TEST_DIR/bin/non_aligned.hex" "00000003: 04 03 02 01"

# 8. Sparse placed sections keep their own address records.
write_source "$tmp/sparse.s" \
    ".section text" \
    ".word 0xAAAAAAAA" \
    ".section data" \
    ".word 0xBBBBBBBB" \
    ".end"
assemble sparse "$tmp/sparse.s"
link_hex sparse -place=text@0x100 -place=data@0x200 "$TEST_DIR/bin/sparse.o"
expect_hex "$TEST_DIR/bin/sparse.hex" "00000100: AA AA AA AA"
expect_hex "$TEST_DIR/bin/sparse.hex" "00000200: BB BB BB BB"

# 9. Overlapping placements are rejected and do not produce output.
write_source "$tmp/overlap.s" \
    ".section text" \
    ".word 0x11111111" \
    ".section data" \
    ".word 0x22222222" \
    ".end"
assemble overlap "$tmp/overlap.s"
capture "$TEST_DIR/bin/link_overlap.log" "$REPO_ROOT/bin/linker" -hex -place=text@0x10 -place=data@0x12 -o "$TEST_DIR/bin/overlap.hex" "$TEST_DIR/bin/overlap.o"
assert_failure "$CAPTURE_STATUS" "linker overlap"
assert_contains "$TEST_DIR/bin/link_overlap.log" "Section placement conflict"
assert_file_not_exists "$TEST_DIR/bin/overlap.hex"

# 10. Unresolved externs are rejected in final hex linking.
write_source "$tmp/unresolved.s" \
    ".extern missing" \
    ".section text" \
    ".word missing" \
    ".end"
assemble unresolved "$tmp/unresolved.s"
capture "$TEST_DIR/bin/link_unresolved.log" "$REPO_ROOT/bin/linker" -hex -o "$TEST_DIR/bin/unresolved.hex" "$TEST_DIR/bin/unresolved.o"
assert_failure "$CAPTURE_STATUS" "linker unresolved"
assert_contains "$TEST_DIR/bin/link_unresolved.log" "Undefined symbol: missing"
assert_file_not_exists "$TEST_DIR/bin/unresolved.hex"

pass "linker_hex_cases"
