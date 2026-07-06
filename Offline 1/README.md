# Symbol Table Implementation using Hash Table

## Overview

This project implements a **Symbol Table** using **Hash Table with Separate Chaining** for compiler design.

The symbol table supports:

* Nested scope management
* Insertion of symbols
* Deletion of symbols
* Symbol lookup
* Printing current scope
* Printing all active scopes

This implementation follows scope-based symbol management where each scope has its own hash table.

---

# Project Structure

```text
2205119_symbol_info.h
2205119_scope_table.h
2205119_symbol_table.h
2205119_main.cpp
2205119_build.sh
input.txt
sample_output.txt
README.md
```

---

# File Description

---

## 1. `2205119_symbol_info.h`

Defines the **SymbolInfo** class.

Each object represents a symbol.

### Data Members:

```cpp
string name;
string type;
SymbolInfo* next;
```

### Purpose:

* Store symbol name
* Store symbol type
* Maintain linked list for collision handling

### Functions:

| Function    | Purpose                 |
| ----------- | ----------------------- |
| Constructor | Creates symbol          |
| Destructor  | Deletes chained symbols |
| `setName()` | Sets symbol name        |
| `setType()` | Sets symbol type        |
| `setNext()` | Sets next pointer       |
| `getName()` | Returns symbol name     |
| `getType()` | Returns symbol type     |
| `getNext()` | Returns next pointer    |

---

## 2. `2205119_scope_table.h`

Defines:

* `SDBMHash()`
* `ScopeTable`

Each scope has its own hash table.

---

### Hash Function

Uses **SDBM Hash Function**:

```cpp
hash = ((str[i]) + (hash << 6) + (hash << 16) - hash) % num_buckets;
```

Purpose:

Converts symbol name into bucket index.

---

### ScopeTable Data Members

```cpp
static int scope_count;
int number_of_buckets;
string id;
ScopeTable* parent_scope;
SymbolInfo** hash_table;
```

---

### Main Functions

---

#### Constructor

Creates:

* Scope ID
* Parent scope connection
* Hash table array

---

#### `Insert()`

Inserts symbol into current scope.

Collision resolution:

```text
Separate Chaining
```

---

#### `Lookup()`

Searches symbol inside current scope.

Returns:

* SymbolInfo pointer if found
* NULL otherwise

---

#### `Remove()`

Deletes symbol from current scope.

Handles:

* Head node deletion
* Middle node deletion
* Tail node deletion

---

#### `print()`

Prints scope table contents.

Supports:

* Normal variables
* Functions
* Structs
* Unions

Special formatting:

Function:

```text
<foo,FUNCTION,INT<==(INT,FLOAT)>
```

Struct:

```text
<car,STRUCT,{(INT,n_doors),(BOOL,is_electric)}>
```

---

## 3. `2205119_symbol_table.h`

Defines **SymbolTable** class.

This manages all scopes.

---

### Data Members

```cpp
int number_of_buckets;
ScopeTable* current;
```

---

### Main Functions

---

#### `enter_scope()`

Creates new scope.

Process:

```text
new_scope.parent = current
current = new_scope
```

---

#### `exit_scope()`

Deletes current scope and returns to parent.

Root scope cannot be removed manually.

---

#### `insert_current_scope()`

Inserts only in current scope.

Duplicate names in same scope are not allowed.

---

#### `remove_current_scope()`

Deletes symbol only from current scope.

---

#### `lookup()`

Search order:

```text
Current → Parent → Parent ...
```

Stops when found.

---

#### `print_this_table()`

Prints only current scope.

---

#### `print_all_tables()`

Prints all active scopes.

With indentation.

---

## 4. `2205119_main.cpp`

Driver file.

Handles:

* Input parsing
* Command execution
* Output generation

---

### Features:

* Reads from input file
* Writes into output file
* Tracks command numbers
* Validates parameter counts for:

```text
L
D
```

---

# Supported Commands

---

## Insert

```text
I <name> <type>
```

Example:

```text
I x VAR
```

Output:

```text
Inserted in ScopeTable# 1 at position 2, 1
```

---

## Lookup

```text
L <name>
```

Example:

```text
L x
```

Searches all active scopes.

---

## Delete

```text
D <name>
```

Example:

```text
D x
```

Deletes only from current scope.

---

## Print Current Scope

```text
P C
```

Prints only current scope.

---

## Print All Scopes

```text
P A
```

Prints all active scopes.

---

## Enter Scope

```text
S
```

Creates new scope.

---

## Exit Scope

```text
E
```

Deletes current scope.

Root scope remains.

---

## Quit

```text
Q
```

Ends program.

Deletes all remaining scopes.

---

# Build Script

File:

```text
2205119_build.sh
```

Contents:

```bash
g++ -fsanitize=address -g -o symbol_table 2205119_main.cpp
./symbol_table input.txt output.txt
```

---

# Compilation

Manual compile:

```bash
g++ -fsanitize=address -g -o symbol_table 2205119_main.cpp
```

---

# Run

```bash
./symbol_table input.txt output.txt
```

Arguments:

| Argument     | Meaning        |
| ------------ | -------------- |
| `input.txt`  | Command input  |
| `output.txt` | Program output |

---

# Input Format

First line:

```text
<number_of_buckets>
```

Example:

```text
7
```

Then commands:

```text
I foo FUNCTION INT INT FLOAT INT
I x VAR
L x
S
P A
Q
```

---

# Output Format

Example:

```text
Cmd 1: I x VAR
    Inserted in ScopeTable# 1 at position 2, 1

Cmd 2: L x
    'x' found in ScopeTable# 1 at position 2, 1
```

---

# Collision Handling

Technique:

```text
Separate Chaining
```

Example:

```text
Bucket 5:
(foo,VAR) -> (bar,VAR)
```

---

# Complexity Analysis

| Operation | Average |
| --------- | ------- |
| Insert    | O(1)    |
| Lookup    | O(1)    |
| Delete    | O(1)    |
| Print     | O(n)    |

Worst case:

```text
O(n)
```

when all keys collide.

---

# Memory Management

Dynamic memory used:

```cpp
new
delete
delete[]
```

Memory checked using:

```text
AddressSanitizer
```

Compile flag:

```bash
-fsanitize=address
```

Used to detect:

* Memory leaks
* Double free
* Invalid access

---

# Author

**Student ID:** 2205119
**Language:** C++
**Data Structure:** Hash Table + Linked List
**Concept:** Compiler Symbol Table
