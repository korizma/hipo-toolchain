#!/usr/bin/env sh
set -eu

TEST_DIR="$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)"
REPO_ROOT="$(CDPATH= cd -- "$TEST_DIR/../../.." && pwd)"
. "$TEST_DIR/../test_lib.sh"

prepare_test_bin "$TEST_DIR"

capture "$TEST_DIR/bin/asm_a.log" \
    "$REPO_ROOT/bin/assembler" -o "$TEST_DIR/bin/a.bin" "$TEST_DIR/a.s"
assert_success "$CAPTURE_STATUS" "assembler a.s"

capture "$TEST_DIR/bin/asm_b.log" \
    "$REPO_ROOT/bin/assembler" -o "$TEST_DIR/bin/b.bin" "$TEST_DIR/b.s"
assert_success "$CAPTURE_STATUS" "assembler b.s"

capture "$TEST_DIR/bin/link.log" \
    "$REPO_ROOT/bin/linker" -relocatable -o "$TEST_DIR/bin/linked.bin" "$TEST_DIR/bin/a.bin" "$TEST_DIR/bin/b.bin"
assert_failure "$CAPTURE_STATUS" "linker"
assert_contains "$TEST_DIR/bin/link.log" "Symbol conflict: duplicate"
assert_file_not_exists "$TEST_DIR/bin/linked.bin"
pass "linker_error_symbol_conflict"
