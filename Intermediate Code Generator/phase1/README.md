# Phase 1: Intermediate Code Generator (Expressions & Basic I/O)

[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg?logo=c%2B%2B)](https://en.cppreference.com/w/cpp/17)
[![ANTLR4](https://img.shields.io/badge/ANTLR-4.13.2-red.svg?logo=java)](https://www.antlr.org/)
[![Target](https://img.shields.io/badge/Target-32--bit%20x86%20FASM-orange.svg)]()
[![Status](https://img.shields.io/badge/Phase%201-Verified-success.svg)]()

Phase 1 of the Intermediate Code Generation (ICG) project implements syntax analysis, code generation, and peephole optimization for the foundational subset of the C grammar specified in [P1_Grammar.pdf](../docs/P1_Grammar.pdf). It compiles basic C programs into 32-bit x86 Linux assembly for the **Flat Assembler (FASM)**.

---

## Scope & Grammar Subset

Per the assignment specification, Phase 1 implements all rules highlighted in `P1_Grammar.pdf`:

- **Variable Declarations**: Global and local integer variable declarations.
- **Assignment Statements**: Assignment expressions (`variable = logic_expression;`).
- **Arithmetic Expressions**:
  - Addition (`+`), subtraction (`-`)
  - Multiplication (`*`), division (`/`), modulus (`%`)
  - Unary plus (`+`), unary minus (`-`)
  - Increment (`++`), decrement (`--`)
- **Console Output**: Built-in procedure `println(ID)` to print 32-bit signed integers to stdout.
- **Peephole Optimization**: Static analysis of generated assembly to remove redundant instructions.

```mermaid
flowchart LR
    A["C Source (input.c)"] --> B["ANTLR4 Lexer/Parser"]
    B --> C["Parse Tree"]
    C --> D["AsmGenerator (AST Visitor)"]
    D --> E["2205119_code.asm"]
    E --> F["Optimizer (Peephole)"]
    F --> G["optimizedcode.asm"]
    G --> H["FASM Assembler"]
    H --> I["Linux ELF Binary"]
```

---

## Directory Layout

```
phase1/
├── grammar/
│   ├── CSubset.g4             # Phase 1 grammar subset
│   └── Lexer.g4               # Lexer specifications
├── include/
│   ├── AsmGenerator.h         # Visitor generating 32-bit x86 assembly
│   ├── Optimizer.h            # Peephole optimizer
│   ├── ScopeTable.h           # Scoped hash table
│   ├── SymbolInfo.h           # Metadata with stack offsets and global flags
│   └── SymbolTable.h          # Multi-scope symbol table
├── src/
│   ├── AsmGenerator.cpp       # Code emission logic
│   ├── Optimizer.cpp          # Peephole pattern recognition passes
│   └── main.cpp               # Driver executable
├── test/
│   ├── input.c                # Standard Phase 1 test program
│   └── test_expressions.c    # Multi-operator expression test
├── Makefile                   # Automated build & test pipeline
├── build.sh                   # Helper script
└── README.md                  # This file
```

---

## Assembly Generation Architecture

1. **Memory Model**:
   - **Global Variables**: Allocated in the data segment (`name dd 0`).
   - **Local Variables**: Allocated on the runtime stack relative to `ebp` (`[ebp - offset]`).
2. **Expression Evaluation**:
   - Postfix/bottom-up stack-based intermediate evaluation using `push eax` and `pop ebx`.
   - Results computed into accumulator `eax`.
3. **Peephole Optimization Passes**:
   - **Redundant Push/Pop**: Identifies and removes consecutive `push reg` immediately followed by `pop reg`.
   - **Redundant Moves**: Filters out unnecessary memory/register reload sequences.
   - **Empty Jumps**: Eliminates jumps to the immediately following line/label.

---

## Build & Test Instructions

### 1. Build
```bash
make all
```

### 2. Run Compiler on Custom File
```bash
make run INPUT=test/input.c
```
This produces:
- `2205119_code.asm`: Unoptimized intermediate code
- `optimizedcode.asm`: Peephole-optimized intermediate code

### 3. Assemble and Execute with FASM
```bash
fasm 2205119_code.asm code.out
chmod +x code.out
./code.out
```

### 4. Run Automated Test Suite
```bash
make test
```
Or run the helper script:
```bash
./build.sh
```
The test target compiles both `test/input.c` and `test/test_expressions.c`, optimizes the code, assembles both versions using FASM, executes both binaries, and validates exact output matching.

### 5. Clean
```bash
make clean
```
