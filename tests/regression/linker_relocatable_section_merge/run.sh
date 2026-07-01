#!/usr/bin/env sh
set -eu

TEST_DIR="$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)"
REPO_ROOT="$(CDPATH= cd -- "$TEST_DIR/../../.." && pwd)"
. "$TEST_DIR/../test_lib.sh"

prepare_test_bin "$TEST_DIR"

capture "$TEST_DIR/bin/asm_left.log" \
    "$REPO_ROOT/bin/assembler" -o "$TEST_DIR/bin/left.bin" "$TEST_DIR/left.s"
assert_success "$CAPTURE_STATUS" "assembler left.s"

capture "$TEST_DIR/bin/asm_right.log" \
    "$REPO_ROOT/bin/assembler" -o "$TEST_DIR/bin/right.bin" "$TEST_DIR/right.s"
assert_success "$CAPTURE_STATUS" "assembler right.s"

capture "$TEST_DIR/bin/link.log" \
    "$REPO_ROOT/bin/linker" -relocatable -o "$TEST_DIR/bin/merged.bin" "$TEST_DIR/bin/left.bin" "$TEST_DIR/bin/right.bin"
assert_success "$CAPTURE_STATUS" "linker"
assert_file_exists "$TEST_DIR/bin/merged.bin"
assert_contains "$TEST_DIR/bin/merged.bin" "#text"
assert_contains "$TEST_DIR/bin/merged.bin" "11 11 11 11 22 22 22 22"
pass "linker_relocatable_section_merge"
