#!/usr/bin/env sh
set -eu

. "$(dirname -- "$0")/test_case_lib.sh"

run_emulator_case "emulator branches and jumps" "$TEST_DIR/branches_jumps.s"

assert_reg 1 0x00000003
assert_reg 2 0x00000003
assert_reg 3 0x00000005
assert_reg 4 0x00000055
assert_reg 10 0x00000000

pass "emulator branches and jumps"
