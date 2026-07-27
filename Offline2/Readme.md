# Lexical Analyzer for a Subset of C Language (Flex + C++)

> **Course:** CSE 310 – Compiler Sessional  
> **Assignment:** Lexical Analysis using Flex  
> **Language:** C++ with Flex (Lex)
> **Author:** Saif Al Islam
> **Student ID:** 2205119

---

# Project Overview

This project implements a **Lexical Analyzer (Scanner)** for a subset of the C programming language using **Flex**.

The lexer reads a C source file character by character, recognizes valid lexemes, converts them into tokens, and stores identifiers/constants inside a **Symbol Table**.

The project also supports

- Keyword detection
- Identifier recognition
- Numeric constants
- Character constants
- String literals
- Operators
- Punctuators
- Comment handling
- Line counting
- Error detection
- Nested scope management using Symbol Tables

This assignment follows the BUET Compiler Sessional specification. :contentReference[oaicite:0]{index=0}

---

# Project Structure

```
2205119.l                 → Main Flex specification
2205119_build.sh          → Build Script
2205119_symbol_info.h     → Symbol Information Class
2205119_scope_table.h     → Scope Table Implementation
2205119_symbol_table.h    → Symbol Table Manager
Assignment on Lexical Analysis.pdf
```

---

# How the Project Works

```
Input C Program
       │
       ▼
 Flex Scanner (.l file)
       │
       ▼
Pattern Matching (Regular Expressions)
       │
       ▼
Recognize Lexeme
       │
       ▼
Generate Token
       │
       ▼
Insert Identifier into Symbol Table
       │
       ▼
Print Token / Log / Errors
```

---

# Theory Behind Lexical Analysis

Lexical Analysis is the **first phase of a compiler**.

Its job is to convert the source program into a sequence of **tokens**.

Example

Source code

```c
int x = 10;
```

becomes

```
<INT>
<ID,x>
<ASSIGNOP,=>
<CONST_INT,10>
<SEMICOLON>
```

Instead of reading characters one by one in later compiler phases, the parser only processes tokens.

---

# Compiler Phases

```
Source Program

      │

Lexical Analyzer
      │

Parser
      │

Semantic Analyzer
      │

Intermediate Code
      │

Optimization
      │

Code Generator
```

Your project implements **only the Lexical Analyzer**.

---

# What is Flex?

Flex (Fast Lexical Analyzer Generator) is a tool that automatically generates a lexical analyzer from regular expressions.

Instead of manually checking every character, we write rules like

```lex
[0-9]+
```

or

```lex
[a-zA-Z_][a-zA-Z0-9_]*
```

Flex converts them into efficient C code.

---

# Symbol Table Concept

A Symbol Table stores information about identifiers.

Example

```c
int x;
float y;
```

Table

| Name | Type |
|------|------|
| x | int |
| y | float |

Whenever an identifier is encountered

- search the symbol table
- if absent → insert
- if present → reuse

---

# Data Structures Used

The project uses

## SymbolInfo

Represents one symbol.

Stores

- Name
- Type
- Pointer to next node

It acts as a linked-list node.

---

## ScopeTable

Each scope owns a hash table.

```
Scope

Bucket 0

Bucket 1

Bucket 2

Bucket 3
```

Every bucket contains a linked list for collision handling.

Operations

- Insert
- Lookup
- Delete
- Print

Hash collisions are resolved using **separate chaining**.

---

## SymbolTable

Manages multiple scopes.

```
Global Scope

      │

Function Scope

      │

If Scope

      │

While Scope
```

Operations

- Enter Scope
- Exit Scope
- Lookup
- Insert
- Remove

---

# Scope Handling

When

```
{
```

appears

```
Enter New Scope
```

When

```
}
```

appears

```
Exit Current Scope
```

Lookup starts from the current scope and moves upward until the symbol is found.

---

# Hash Function

The project uses the **SDBM Hash Function**.

```
hash = c + (hash << 6)
           + (hash << 16)
           - hash
```

Advantages

- Fast
- Good distribution
- Few collisions

---

# Symbol Lookup

```
Current Scope

      │
      ▼

Found ?

YES → Return

NO

      │

Parent Scope

      │

Found ?

YES → Return

NO

Continue

↓

Not Found
```

---

# Token Categories

The lexer recognizes

### Keywords

```
if
else
while
for
int
float
char
return
```

---

### Identifiers

```
sum

counter

student_name

temp123
```

---

### Integer Constants

```
5

100

999
```

---

### Floating Constants

```
3.14

1E10

0.25

12.3E-2
```

---

### Character Constants

```
'a'

'\n'

'\t'
```

---

### String Literals

```
"Hello"

"Compiler"
```

---

### Operators

```
+

-

*

/

%

=

==

!=

&&

||

++

--
```

---

### Punctuators

```
(

)

{

}

[

]

,

;
```

---

# Error Detection

The lexer detects lexical errors such as

- Multiple decimal points
- Invalid numeric suffixes
- Invalid identifier prefixes
- Multi-character constants
- Unfinished character literals
- Empty character constants
- Unfinished strings
- Unfinished comments
- Unrecognized characters

Each error is reported with its corresponding line number.

---

# How Symbol Table Works

Suppose input is

```c
int a;
float b;

a = 5;
```

Processing

```
INT

↓

Keyword

↓

No insertion
```

```
a

↓

Identifier

↓

Insert into Symbol Table
```

```
5

↓

Constant

↓

Insert (if required)
```

---

# Build Instructions

## Install Flex

Ubuntu / Debian

```bash
sudo apt install flex
```

Arch Linux

```bash
sudo pacman -S flex
```

---

# Build Manually

Generate scanner

```bash
flex 2205119.l
```

Compile

```bash
g++ lex.yy.c -o lexer
```

Run

```bash
./lexer input1.txt
```

These are the same steps provided by the project's build script. :contentReference[oaicite:1]{index=1}

---

# Build Using Script

Give execution permission

```bash
chmod +x 2205119_build.sh
```

Run

```bash
./2205119_build.sh
```

---

# Expected Output

Input

```c
int main(){

int a=10;

float x=5.5;

}
```

Example Tokens

```
<INT>

<ID,main>

<LPAREN>

<RPAREN>

<LCURL>

<INT>

<ID,a>

<ASSIGNOP,=>

<CONST_INT,10>

<SEMICOLON>

<FLOAT>

<ID,x>

<ASSIGNOP,=>

<CONST_FLOAT,5.5>

<SEMICOLON>

<RCURL>
```

---

# Important Classes

## SymbolInfo

Responsible for

- Name storage
- Type storage
- Linked-list connection

---

## ScopeTable

Responsible for

- Hash Table
- Insert
- Lookup
- Delete
- Print
- Scope ID

---

## SymbolTable

Responsible for

- Managing nested scopes
- Searching across scopes
- Creating scopes
- Destroying scopes

---

# Time Complexity

| Operation | Complexity |
|-----------|------------|
| Insert | O(1) Average |
| Lookup | O(1) Average |
| Delete | O(1) Average |
| Enter Scope | O(1) |
| Exit Scope | O(1) |

Worst case

```
O(n)
```

when every symbol hashes into the same bucket.

---

# Learning Outcomes

After completing this project, you should understand

- Compiler design basics
- Lexical analysis
- Tokens and lexemes
- Regular expressions
- Flex programming
- Hash tables
- Symbol tables
- Nested scopes
- Collision handling
- Scanner generation
- Error reporting

---

# Conclusion

This project demonstrates how a compiler's lexical analyzer transforms raw C source code into meaningful tokens while maintaining a symbol table for identifiers and constants. By combining **Flex** for pattern matching with custom C++ data structures for symbol and scope management, it forms the foundation for later compiler phases such as parsing, semantic analysis, optimization, and code generation.
