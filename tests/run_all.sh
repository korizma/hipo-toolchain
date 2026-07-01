#!/usr/bin/env sh
set -eu

ROOT_DIR="$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)"
TEST_ROOT="$ROOT_DIR/tests/regression"

if [ "${SS_TEST_SKIP_BUILD:-0}" != "1" ]; then
    make -C "$ROOT_DIR" all
fi

status=0

for test_script in $(find "$TEST_ROOT" -mindepth 2 -maxdepth 2 -name run.sh | sort); do
    test_name="$(basename "$(dirname "$test_script")")"
    printf '== %s ==\n' "$test_name"

    if sh "$test_script"; then
        :
    else
        status=1
    fi
done

exit "$status"
