#!/bin/bash
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

echo "Generating Lexer from Flex specification..."
flex -o src/lex.yy.c src/lexer.l

echo "Compiling Lexer binary..."
g++ -std=c++17 -Wall -Wextra -Iinclude src/lex.yy.c -o lexer

INPUT="${1:-test/input1.txt}"
TOKEN_OUT="${2:-token.txt}"
LOG_OUT="${3:-log.txt}"

echo "Running lexer with input: $INPUT -> ($TOKEN_OUT, $LOG_OUT) ..."
./lexer "$INPUT" "$TOKEN_OUT" "$LOG_OUT"
echo "Execution completed successfully."
