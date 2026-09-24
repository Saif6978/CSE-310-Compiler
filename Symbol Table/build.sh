#!/bin/bash
set -e

# Compile with AddressSanitizer and C++17
echo "Compiling Symbol Table..."
g++ -std=c++17 -Wall -Wextra -fsanitize=address -g -Iinclude src/main.cpp -o symbol_table

# Run with provided arguments or default test files
INPUT="${1:-test/input.txt}"
OUTPUT="${2:-test/output.txt}"

echo "Running symbol_table with $INPUT -> $OUTPUT ..."
./symbol_table "$INPUT" "$OUTPUT"

# Verify against sample_output.txt if running default test
if [ "$INPUT" = "test/input.txt" ] && [ -f "test/sample_output.txt" ]; then
    if diff -u "$OUTPUT" test/sample_output.txt > /dev/null; then
        echo "Verification PASSED: output matches sample_output.txt exactly!"
    else
        echo "Verification FAILED: output mismatch!"
        diff -u "$OUTPUT" test/sample_output.txt
        exit 1
    fi
fi
