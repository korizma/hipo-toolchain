#!/usr/bin/env sh
set -eu

. "$(dirname -- "$0")/test_case_lib.sh"

run_emulator_case "emulator load/store variants" "$TEST_DIR/load_store_variants.s"

assert_reg 2 0x00000055
assert_reg 5 0x00000123
assert_reg 6 0x00000055

pass "emulator load/store variants"
