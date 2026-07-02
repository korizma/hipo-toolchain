#!/usr/bin/env sh
set -eu

. "$(dirname -- "$0")/test_case_lib.sh"

run_emulator_case "emulator stack call ret" "$TEST_DIR/stack_call_ret.s"

assert_reg 3 0x00000222
assert_reg 4 0x00000111
assert_reg 5 0x00000007
assert_reg 7 0x00000055
assert_reg 14 0x00000700

pass "emulator stack call ret"
