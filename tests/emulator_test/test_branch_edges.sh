#!/usr/bin/env sh
set -eu

. "$(dirname -- "$0")/test_case_lib.sh"

run_emulator_case "emulator branch edges" "$TEST_DIR/branch_edges.s"

assert_reg 3 0x00000001
assert_reg 4 0x00000055
assert_reg 10 0x00000000

pass "emulator branch edges"

