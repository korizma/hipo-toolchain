#!/usr/bin/env sh
set -eu

ROOT_DIR="$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)"
TEST_ROOT="$ROOT_DIR/tests/emulator_test"

if [ "${SS_TEST_SKIP_BUILD:-0}" != "1" ]; then
    make -C "$ROOT_DIR" all
fi

status=0

for test_script in $(find "$TEST_ROOT" -maxdepth 1 -name 'test*.sh' ! -name 'test_case_lib.sh' | sort); do
    test_name="$(basename "$test_script" .sh)"
    printf '== %s ==\n' "$test_name"

    if SS_TEST_SKIP_BUILD=1 sh "$test_script"; then
        :
    else
        status=1
    fi
done

exit "$status"
