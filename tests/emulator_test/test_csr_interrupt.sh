#!/usr/bin/env sh
set -eu

. "$(dirname -- "$0")/test_case_lib.sh"

run_emulator_case "emulator csr and interrupt" "$TEST_DIR/csr_interrupt.s"

assert_reg 4 0x000000FF
assert_reg 5 0x00000004
assert_reg 6 0x000000FE
assert_reg 7 0x000006F8
assert_reg 10 0x00000000
assert_reg 11 0x00000000

pass "emulator csr and interrupt"
