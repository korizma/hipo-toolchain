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
assert_matches "$TEST_DIR/bin/output.bin" '^[0-9]+:[[:space:]]+4[[:space:]]+[0-9-]+[[:space:]]+NOTYP[[:space:]]+LOC[[:space:]]+[0-9]+[[:space:]]+start_plus_four$'
pass "assembler_equ_symbol"
