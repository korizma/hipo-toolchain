#!/usr/bin/env sh
set -eu

. "$(dirname -- "$0")/test_case_lib.sh"

run_emulator_case "emulator memory addressing" "$TEST_DIR/memory_addressing.s"

assert_reg 2 0x00000123
assert_reg 5 0x00000456
assert_reg 7 0x00000789
assert_reg 9 0x00000055
assert_reg 10 0x00000123

pass "emulator memory addressing"
