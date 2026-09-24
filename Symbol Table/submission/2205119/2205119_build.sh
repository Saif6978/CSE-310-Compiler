#!/bin/bash
g++ -std=c++17 -fsanitize=address -g -o symbol_table 2205119_main.cpp
if [ $? -eq 0 ]; then
    IN="${1:-input.txt}"
    OUT="${2:-output.txt}"
    ./symbol_table "$IN" "$OUT"
fi
