#!/usr/bin/env sh
set -eu

TEST_DIR="$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)"
REPO_ROOT="$(CDPATH= cd -- "$TEST_DIR/../../.." && pwd)"
. "$TEST_DIR/../test_lib.sh"

prepare_test_bin "$TEST_DIR"

capture "$TEST_DIR/bin/assembler.log" \
    "$REPO_ROOT/bin/assembler" -o "$TEST_DIR/bin/output.bin" "$TEST_DIR/input.s"

assert_failure "$CAPTURE_STATUS" "assembler"
assert_contains "$TEST_DIR/bin/assembler.log" "Undefined symbol: missing"
assert_file_not_exists "$TEST_DIR/bin/output.bin"
pass "assembler_error_undefined_symbol"
