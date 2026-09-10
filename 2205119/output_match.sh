#!/bin/bash


BASE_DIR="$(cd "$(dirname "$0")" && pwd)"

cd "$BASE_DIR" || exit 1


INPUT_DIR="./input"
EXPECTED_FILE="./input/output.txt"


TESTS=(
    "test1_i"
    "test2_i"
    "test3_i"
    "test4_i"
    "test5_i"
    "test6_i"
    "test7_i"
    "recursion1_i"
    "recursion2_i"
)


PASS=0
FAIL=0



echo "======================================"
echo " Running Compiler Test Suite"
echo "======================================"



# activate antlr environment

if [ -f "antlr4_venv/bin/activate" ]
then
    source antlr4_venv/bin/activate
fi



# Extract expected output from output.txt

get_expected()
{
    TEST=$1

    awk -v test="${TEST}.out" '

    BEGIN{
        found=0
    }


    $0 == test {
        found=1
        next
    }


    found && /^[-]+$/ {
        next
    }


    found && /^[a-zA-Z0-9_]+\.out$/ {
        exit
    }


    found {
        print
    }


    ' "$EXPECTED_FILE"
}




for TEST in "${TESTS[@]}"
do

    echo
    echo "--------------------------------------"
    echo "Testing: $TEST"
    echo "--------------------------------------"



    INPUT_FILE="$INPUT_DIR/$TEST.c"



    if [ ! -f "$INPUT_FILE" ]
    then
        echo "❌ Missing input file: $INPUT_FILE"
        FAIL=$((FAIL+1))
        continue
    fi



    ./clean-script.sh >/dev/null 2>&1



    echo "[1] Generating assembly..."

    ./run-script.sh "$INPUT_FILE"



    if [ $? -ne 0 ]
    then
        echo "❌ Compiler failed"
        FAIL=$((FAIL+1))
        continue
    fi




    echo "[2] Assembling..."

    fasm 2205119_code.asm code.out >/dev/null



    if [ $? -ne 0 ]
    then
        echo "❌ FASM failed"
        FAIL=$((FAIL+1))
        continue
    fi




    chmod +x code.out



    echo "[3] Running executable..."

    ACTUAL_FILE="${TEST}_actual.txt"
    EXPECTED_OUT="${TEST}_expected.txt"



    ./code.out | sed '/^[[:space:]]*$/d' > "$ACTUAL_FILE"



    if [ $? -ne 0 ]
    then
        echo "❌ Runtime crash"
        FAIL=$((FAIL+1))
        continue
    fi



    get_expected "$TEST" | sed '/^[[:space:]]*$/d' > "$EXPECTED_OUT"



    echo
    echo "Generated Output:"
    cat "$ACTUAL_FILE"


    echo
    echo "Expected Output:"
    cat "$EXPECTED_OUT"


    echo



    diff -w "$ACTUAL_FILE" "$EXPECTED_OUT" >/dev/null



    if [ $? -eq 0 ]
    then
        echo "✅ PASS"
        PASS=$((PASS+1))

    else

        echo "❌ FAIL"

        echo
        echo "Difference:"
        diff -w "$ACTUAL_FILE" "$EXPECTED_OUT"

        FAIL=$((FAIL+1))

    fi


done




echo
echo "======================================"
echo " Test Summary"
echo "======================================"

echo "Passed : $PASS"
echo "Failed : $FAIL"
echo "Total  : ${#TESTS[@]}"

echo "======================================"



if [ $FAIL -eq 0 ]
then
    echo "🎉 All tests passed"
else
    echo "⚠ Some tests failed"
fi