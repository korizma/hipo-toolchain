#!/usr/bin/env sh
set -eu

ROOT_DIR="$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)"
TEST_DIR="$ROOT_DIR/tests/emulator_test"

. "$ROOT_DIR/tests/regression/test_lib.sh"

run_emulator_case()
{
    case_name="$1"
    source_file="$2"

    if [ "${SS_TEST_SKIP_BUILD:-0}" != "1" ]; then
        make -C "$ROOT_DIR" all
    fi

    tmp="$(mktemp -d)"
    trap 'rm -rf "$tmp"' EXIT

    capture "$tmp/assembler.out" "$ROOT_DIR/bin/assembler" -o "$tmp/main.o" "$source_file"
    assert_success "$CAPTURE_STATUS" "$case_name assembler"
    assert_file_exists "$tmp/main.o"

    capture "$tmp/linker.out" "$ROOT_DIR/bin/linker" -hex -place=text@0x40000000 -o "$tmp/program.hex" "$tmp/main.o"
    assert_success "$CAPTURE_STATUS" "$case_name linker"
    assert_file_exists "$tmp/program.hex"

    if command -v timeout >/dev/null 2>&1; then
        capture "$tmp/emulator.out" timeout 5 "$ROOT_DIR/bin/emulator" "$tmp/program.hex"
    else
        capture "$tmp/emulator.out" "$ROOT_DIR/bin/emulator" "$tmp/program.hex"
    fi
    assert_success "$CAPTURE_STATUS" "$case_name emulator"
}

assert_reg()
{
    reg="$1"
    expected="$2"
    assert_matches "$tmp/emulator.out" "(^|[[:space:]])r${reg}[[:space:]]*[:=][[:space:]]*${expected}([^0-9A-Fa-f]|$)"
}
