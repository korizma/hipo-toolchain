#!/usr/bin/env sh
set -eu

. "$(dirname -- "$0")/test_case_lib.sh"

run_emulator_case "emulator arithmetic and logic" "$TEST_DIR/arithmetic_logic.s"

assert_reg 1 0x00000003
assert_reg 3 0x00000010
assert_reg 4 0x00000034
assert_reg 6 0x00000024
assert_reg 8 0xFFFFFFEE
assert_reg 10 0x00000003

pass "emulator arithmetic and logic"
