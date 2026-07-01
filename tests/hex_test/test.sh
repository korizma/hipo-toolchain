set -e
tmp="$(mktemp -d)"

make all
./bin/assembler -o "$tmp/source1.bin" ./tests/relocatable_test/source1.s
./bin/assembler -o "$tmp/source2.bin" ./tests/relocatable_test/source2.s
./bin/assembler -o "$tmp/source3.bin" ./tests/relocatable_test/source3.s

# echo "Source files:"
# cat "$tmp/source1.bin"
# cat "$tmp/source2.bin"
# cat "$tmp/source3.bin"

./bin/linker -o "$tmp/partly_linked.bin" "$tmp/source1.bin" "$tmp/source2.bin" -relocatable

echo "Partly linked file:"
cat "$tmp/partly_linked.bin"

./bin/linker -o "$tmp/linked.bin" "$tmp/partly_linked.bin" "$tmp/source3.bin" -hex -place=glavni@0x2026

echo "Linked file:"
cat "$tmp/linked.bin"