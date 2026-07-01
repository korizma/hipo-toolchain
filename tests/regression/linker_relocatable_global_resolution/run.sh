#!/usr/bin/env sh
set -eu

TEST_DIR="$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)"
REPO_ROOT="$(CDPATH= cd -- "$TEST_DIR/../../.." && pwd)"
. "$TEST_DIR/../test_lib.sh"

prepare_test_bin "$TEST_DIR"

capture "$TEST_DIR/bin/asm_ref.log" \
    "$REPO_ROOT/bin/assembler" -o "$TEST_DIR/bin/ref.bin" "$TEST_DIR/ref.s"
assert_success "$CAPTURE_STATUS" "assembler ref.s"

capture "$TEST_DIR/bin/asm_def.log" \
    "$REPO_ROOT/bin/assembler" -o "$TEST_DIR/bin/def.bin" "$TEST_DIR/def.s"
assert_success "$CAPTURE_STATUS" "assembler def.s"

capture "$TEST_DIR/bin/link.log" \
    "$REPO_ROOT/bin/linker" -relocatable -o "$TEST_DIR/bin/linked.bin" "$TEST_DIR/bin/def.bin" "$TEST_DIR/bin/ref.bin"
assert_success "$CAPTURE_STATUS" "linker"
assert_file_exists "$TEST_DIR/bin/linked.bin"
assert_contains "$TEST_DIR/bin/linked.bin" "#text"
assert_contains "$TEST_DIR/bin/linked.bin" "00 00 00 00 00 00 00 00"
assert_matches "$TEST_DIR/bin/linked.bin" '^[0-9]+:[[:space:]]+0[[:space:]]+0[[:space:]]+NOTYP[[:space:]]+GLOB[[:space:]]+[0-9]+[[:space:]]+target$'
assert_matches "$TEST_DIR/bin/linked.bin" '^[0-9]+:[[:space:]]+4[[:space:]]+0[[:space:]]+NOTYP[[:space:]]+GLOB[[:space:]]+[0-9]+[[:space:]]+start$'
assert_contains "$TEST_DIR/bin/linked.bin" "0000000000000004	R_HIPO_32"

capture "$TEST_DIR/bin/relink.log" \
    "$REPO_ROOT/bin/linker" -relocatable -o "$TEST_DIR/bin/relinked.bin" "$TEST_DIR/bin/linked.bin"
assert_success "$CAPTURE_STATUS" "relinker"
assert_file_exists "$TEST_DIR/bin/relinked.bin"
assert_contains "$TEST_DIR/bin/relinked.bin" "#.rela.text"
pass "linker_relocatable_global_resolution"
