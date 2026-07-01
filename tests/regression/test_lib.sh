#!/usr/bin/env sh

fail()
{
    printf 'FAIL: %s\n' "$*" >&2
    exit 1
}

pass()
{
    printf 'PASS: %s\n' "$1"
}

prepare_test_bin()
{
    test_dir="$1"
    rm -rf "$test_dir/bin"
    mkdir -p "$test_dir/bin"
}

capture()
{
    output_file="$1"
    shift

    set +e
    "$@" >"$output_file" 2>&1
    CAPTURE_STATUS=$?
    set -e
}

assert_success()
{
    status="$1"
    context="$2"

    if [ "$status" -ne 0 ]; then
        fail "$context exited with status $status"
    fi
}

assert_failure()
{
    status="$1"
    context="$2"

    if [ "$status" -eq 0 ]; then
        fail "$context unexpectedly succeeded"
    fi
}

assert_file_exists()
{
    file="$1"

    if [ ! -f "$file" ]; then
        fail "expected file to exist: $file"
    fi
}

assert_file_not_exists()
{
    file="$1"

    if [ -e "$file" ]; then
        fail "expected file not to exist: $file"
    fi
}

assert_contains()
{
    file="$1"
    text="$2"

    if ! grep -Fq "$text" "$file"; then
        printf '%s\n' "---- $file ----" >&2
        sed -n '1,160p' "$file" >&2
        fail "expected '$file' to contain: $text"
    fi
}

assert_matches()
{
    file="$1"
    regex="$2"

    if ! grep -Eq "$regex" "$file"; then
        printf '%s\n' "---- $file ----" >&2
        sed -n '1,160p' "$file" >&2
        fail "expected '$file' to match regex: $regex"
    fi
}
