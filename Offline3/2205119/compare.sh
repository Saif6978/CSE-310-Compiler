#!/bin/bash
set -u

cd "$(dirname "$0")"

EXPECTED_LOG_DIR="sample_output/log"
EXPECTED_ERROR_DIR="sample_output/error"
ACTUAL_LOG_DIR="output/log"
ACTUAL_ERROR_DIR="output/error"
DIFF_DIR="comparison_diffs"

mkdir -p "$DIFF_DIR"
rm -f "$DIFF_DIR"/*.diff 2>/dev/null || true

pass_count=0
fail_count=0

compare_file() {
    local label="$1"
    local expected="$2"
    local actual="$3"
    local diff_file="$4"

    if [[ ! -f "$actual" ]]; then
        echo "[FAIL] $label -- generated file is missing: $actual"
        echo "Generated file missing: $actual" > "$diff_file"
        fail_count=$((fail_count + 1))
        return
    fi

    # The supplied set has empty error files for cases 1 and 3. If an empty
    # expected file is not physically present, /dev/null is equivalent.
    local expected_for_diff="$expected"
    if [[ ! -f "$expected" ]]; then
        expected_for_diff="/dev/null"
    fi

    if cmp -s "$expected_for_diff" "$actual"; then
        echo "[PASS] $label"
        pass_count=$((pass_count + 1))
        rm -f "$diff_file"
    else
        echo "[FAIL] $label"
        diff -u \
            --label "expected/$label" \
            --label "actual/$label" \
            "$expected_for_diff" "$actual" > "$diff_file" || true
        echo "       Diff: $diff_file"
        fail_count=$((fail_count + 1))
    fi
}

echo "=============================================="
echo " Comparing generated output with sample files"
echo "=============================================="

for i in 1 2 3 4 5; do
    echo
    echo "Input $i"
    echo "-------"

    compare_file \
        "log${i}.txt" \
        "$EXPECTED_LOG_DIR/log${i}.txt" \
        "$ACTUAL_LOG_DIR/log${i}.txt" \
        "$DIFF_DIR/log${i}.diff"

    compare_file \
        "error${i}.txt" \
        "$EXPECTED_ERROR_DIR/error${i}.txt" \
        "$ACTUAL_ERROR_DIR/error${i}.txt" \
        "$DIFF_DIR/error${i}.diff"
done

echo
echo "=============================================="
echo "Result: $pass_count passed, $fail_count failed"
echo "=============================================="

if [[ $fail_count -eq 0 ]]; then
    echo "All generated log/error files exactly match the samples."
    exit 0
else
    echo "Differences were found. Open files inside: $DIFF_DIR/"
    exit 1
fi
