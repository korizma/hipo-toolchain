#!/usr/bin/env sh
set -eu

. "$(dirname -- "$0")/test_case_lib.sh"

run_emulator_case "emulator data section access" "$TEST_DIR/data_section_access.s"

assert_reg 1 0x13579BDF
assert_reg 3 0x13579BDF

pass "emulator data section access"

