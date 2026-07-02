#!/usr/bin/env sh
set -eu

ROOT_DIR="$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)"
TEST_DIR="$ROOT_DIR/tests/emulator_test"

. "$ROOT_DIR/tests/regression/test_lib.sh"

if [ "${SS_TEST_SKIP_BUILD:-0}" != "1" ]; then
    make -C "$ROOT_DIR" all
fi

tmp="$(mktemp -d)"
trap 'rm -rf "$tmp"' EXIT

capture "$tmp/assembler.out" "$ROOT_DIR/bin/assembler" -o "$tmp/main.o" "$TEST_DIR/main.s"
assert_success "$CAPTURE_STATUS" "assembler"
assert_file_exists "$tmp/main.o"

capture "$tmp/linker.out" "$ROOT_DIR/bin/linker" -hex -place=text@0x40000000 -o "$tmp/program.hex" "$tmp/main.o"
assert_success "$CAPTURE_STATUS" "linker"
assert_file_exists "$tmp/program.hex"

if command -v timeout >/dev/null 2>&1; then
    capture "$tmp/emulator.out" timeout 5 "$ROOT_DIR/bin/emulator" "$tmp/program.hex"
else
    capture "$tmp/emulator.out" "$ROOT_DIR/bin/emulator" "$tmp/program.hex"
fi
assert_success "$CAPTURE_STATUS" "emulator"

assert_reg()
{
    reg="$1"
    expected="$2"
    assert_matches "$tmp/emulator.out" "(^|[[:space:]])r${reg}[[:space:]]*[:=][[:space:]]*${expected}([^0-9A-Fa-f]|$)"
}

assert_reg 0 0x00000000
assert_reg 1 0x00000007
assert_reg 2 0x00000006
assert_reg 3 0x00000004
assert_reg 4 0x000000F0
assert_reg 5 0x00000024
assert_reg 6 0xFFFFFFEE
assert_reg 7 0x00000055
assert_reg 8 0x00000234
assert_reg 9 0x00000234
assert_reg 10 0x00000234
assert_reg 11 0x00000003
assert_reg 12 0x00000345
assert_reg 13 0x00000345
assert_reg 14 0x00000700

pass "emulator basic instruction sample"
