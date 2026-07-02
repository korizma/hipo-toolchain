#!/usr/bin/env sh
set -eu

. "$(dirname -- "$0")/test_case_lib.sh"

run_emulator_case "emulator nested calls" "$TEST_DIR/nested_calls.s"

assert_reg 1 0x00000007
assert_reg 4 0x00000700

pass "emulator nested calls"

