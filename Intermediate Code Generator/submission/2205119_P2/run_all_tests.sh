#!/bin/bash

BASE_DIR="$(cd "$(dirname "$0")" && pwd)"

cd "$BASE_DIR" || exit 1


TEST_DIR="./input"

TESTS=(
    "test1_i.c"
    "test2_i.c"
    "test3_i.c"
    "test4_i.c"
    "test5_i.c"
    "test6_i.c"
    "test7_i.c"
    "func.c"
    "loop.c"
    "recursion1_i.c"
    "recursion2_i.c"
    "exp.c"
)


PASS=0
FAIL=0


echo "======================================"
echo " Running Compiler Test Suite"
echo "======================================"



# activate antlr environment

if [ -f "antlr4_venv/bin/activate" ]; then
    source antlr4_venv/bin/activate
else
    echo "ANTLR virtual environment not found"
    exit 1
fi



for TEST in "${TESTS[@]}"
do

    echo
    echo "--------------------------------------"
    echo "Testing: $TEST"
    echo "--------------------------------------"


    ./clean-script.sh >/dev/null 2>&1


    ./run-script.sh "$TEST_DIR/$TEST"


    if [ $? -ne 0 ]
    then
        echo "❌ Compiler failed"
        FAIL=$((FAIL+1))
        continue
    fi



    if [ ! -f "2205119_code.asm" ]
    then
        echo "❌ Assembly file not generated"
        FAIL=$((FAIL+1))
        continue
    fi



    fasm 2205119_code.asm code.out >/dev/null


    if [ $? -ne 0 ]
    then
        echo "❌ FASM failed"
        FAIL=$((FAIL+1))
        continue
    fi



    chmod +x code.out


    echo "Output:"
    ./code.out


    if [ $? -eq 0 ]
    then
        echo "✅ PASS"
        PASS=$((PASS+1))
    else
        echo "❌ Runtime failure"
        FAIL=$((FAIL+1))
    fi


done



echo
echo "======================================"
echo "Summary"
echo "======================================"

echo "Passed : $PASS"
echo "Failed : $FAIL"