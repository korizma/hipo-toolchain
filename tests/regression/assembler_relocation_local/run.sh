#!/usr/bin/env sh
set -eu

TEST_DIR="$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)"
REPO_ROOT="$(CDPATH= cd -- "$TEST_DIR/../../.." && pwd)"
. "$TEST_DIR/../test_lib.sh"

prepare_test_bin "$TEST_DIR"

capture "$TEST_DIR/bin/assembler.log" \
    "$REPO_ROOT/bin/assembler" -o "$TEST_DIR/bin/output.bin" "$TEST_DIR/input.s"

assert_success "$CAPTURE_STATUS" "assembler"
assert_file_exists "$TEST_DIR/bin/output.bin"
assert_contains "$TEST_DIR/bin/output.bin" "#.rela.text"
assert_contains "$TEST_DIR/bin/output.bin" "0000000000000000	R_HIPO_32	0	0"
pass "assembler_relocation_local"
