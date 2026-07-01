#!/usr/bin/env sh
set -eu

TEST_DIR="$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)"
REPO_ROOT="$(CDPATH= cd -- "$TEST_DIR/../../.." && pwd)"
. "$TEST_DIR/../test_lib.sh"

prepare_test_bin "$TEST_DIR"

capture "$TEST_DIR/bin/asm_extern.log" \
    "$REPO_ROOT/bin/assembler" -o "$TEST_DIR/bin/extern_ref.bin" "$TEST_DIR/extern_ref.s"
assert_success "$CAPTURE_STATUS" "assembler extern_ref.s"

capture "$TEST_DIR/bin/asm_local.log" \
    "$REPO_ROOT/bin/assembler" -o "$TEST_DIR/bin/local_def.bin" "$TEST_DIR/local_def.s"
assert_success "$CAPTURE_STATUS" "assembler local_def.s"

capture "$TEST_DIR/bin/link.log" \
    "$REPO_ROOT/bin/linker" -relocatable -o "$TEST_DIR/bin/linked.bin" "$TEST_DIR/bin/local_def.bin" "$TEST_DIR/bin/extern_ref.bin"
assert_failure "$CAPTURE_STATUS" "linker"
assert_contains "$TEST_DIR/bin/link.log" "Extern local conflict: local_only"
assert_file_not_exists "$TEST_DIR/bin/linked.bin"
pass "linker_error_extern_local_conflict"
