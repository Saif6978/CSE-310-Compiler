#!/bin/bash
set -euo pipefail

# Run from the project root.
cd "$(dirname "$0")"

ANTLR_VERSION="4.13.2"
ANTLR_INCLUDE="/usr/local/include/antlr4-runtime"
ANTLR_LIB="/usr/local/lib"

mkdir -p output/log output/error output/lex
rm -f output/log/log{1..5}.txt
rm -f output/error/error{1..5}.txt
rm -f output/lex/lexLog{1..5}.txt

# Generate ANTLR C++ lexer/parser/visitor sources.
antlr4 -v "$ANTLR_VERSION" -Dlanguage=Cpp -visitor -no-listener CSubset.g4

# Build.
g++ -std=c++17 -w \
    -I"$ANTLR_INCLUDE" \
    *.cpp \
    -L"$ANTLR_LIB" \
    -lantlr4-runtime \
    -pthread \
    -o compiler.out

# Run all five sample inputs in one invocation.
LD_LIBRARY_PATH="$ANTLR_LIB${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}" \
./compiler.out \
    sample_input/input1.txt \
    sample_input/input2.txt \
    sample_input/input3.txt \
    sample_input/input4.txt \
    sample_input/input5.txt

echo
echo "Batch run complete."
echo "Generated logs   : output/log/"
echo "Generated errors : output/error/"
