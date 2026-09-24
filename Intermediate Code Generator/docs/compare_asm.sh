#!/bin/bash

# Check arguments
if [ $# -ne 2 ]; then
    echo "Usage: ./compare_asm.sh file1.asm file2.asm"
    exit 1
fi

ASM1=$1
ASM2=$2

# Generate executable names
EXE1="${ASM1%.asm}.out"
EXE2="${ASM2%.asm}.out"

echo "Assembling $ASM1..."

fasm "$ASM1" "$EXE1"

if [ $? -ne 0 ]; then
    echo "Failed assembling $ASM1"
    exit 1
fi


echo "Assembling $ASM2..."

fasm "$ASM2" "$EXE2"

if [ $? -ne 0 ]; then
    echo "Failed assembling $ASM2"
    exit 1
fi


# Give execution permission
chmod +x "$EXE1"
chmod +x "$EXE2"


# Remove previous outputs
rm -f output1.txt output2.txt


echo "Running executables..."


./"$EXE1" > output1.txt
STATUS1=$?


./"$EXE2" > output2.txt
STATUS2=$?



# Check execution failure

if [ $STATUS1 -ne 0 ]; then
    echo "Reference executable crashed or failed"
    exit 1
fi


if [ $STATUS2 -ne 0 ]; then
    echo "Generated executable crashed or failed"
    exit 1
fi



echo ""
echo "========== OUTPUT 1 =========="
cat output1.txt


echo ""
echo "========== OUTPUT 2 =========="
cat output2.txt



echo ""
echo "========== COMPARISON =========="


diff -u output1.txt output2.txt


if [ $? -eq 0 ]; then
    echo "✅ Outputs are identical"
else
    echo "❌ Outputs are different"
fi