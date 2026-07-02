#!/usr/bin/env sh
set -eu

. "$(dirname -- "$0")/test_case_lib.sh"

run_emulator_case "emulator push/pop order" "$TEST_DIR/push_pop_order.s"

assert_reg 4 0x00000030
assert_reg 5 0x00000020
assert_reg 6 0x00000010
assert_reg 7 0x00000700

pass "emulator push/pop order"

