# CSE 310 – Compiler Sessional: Practice Assignment 2 (Lexical Analysis)
### Context, Problem Statement, and Implementation Plan

---

## 1. Context / Background

Lexical analysis is the **first phase of a compiler**. It scans the source program as a raw stream of characters and groups them into meaningful chunks called **lexemes**, each of which is classified into a **token** of the form `<TOKEN_TYPE, lexeme>`. These tokens are later consumed by the parser to build an abstract syntax tree.

Example: for the source fragment `int x=5;` the lexer should output:

```
<INT> <ID,x> <ASSIGNOP,=> <CONST_NUM,5> <SEMICOLON>
```

The assignment builds on a **symbol table** implementation (from Assignment 1) and asks you to write a scanner for a subset of C using **Flex (Fast Lexical Analyzer Generator)**.

### Key Flex concepts you need (from the lecture)

- **Flex is a lexer generator** — you write patterns + actions in a `.l` file and Flex generates `lex.yy.c`, which you compile into a working scanner.
- **A `.l` file has 3 sections separated by `%%`:**
  1. **Definition section** – options (`%option noyywrap`, `yylineno`), named subpatterns (e.g. `Letter [a-zA-Z]`), `%{ ... %}` block for C/C++ includes, global variables, helper functions, and symbol-table object declaration.
  2. **Rules section** – `pattern  { action }` pairs. Patterns must start at column 0 (no leading whitespace, or Flex treats it as C code). The matched text is available in `yytext`.
  3. **Subroutine section** – plain C code, typically the `main()` function that opens the input file, points `yyin` to it, sets `yylineno = 1`, and calls `yylex()`.
- **Ambiguity/order rules**: Flex picks the *longest match*; if two rules match the same longest length, the rule **written first** wins. So rule ordering matters (e.g., keywords before generic identifiers, or use a symbol-table lookup to reclassify identifiers that turn out to be keywords).
- **Start states** (`%x` exclusive / `%s` inclusive, `BEGIN`) let you activate different sets of rules in different contexts — useful for multi-line comments and multi-line strings, where you enter a special state until you see the terminating sequence, then `BEGIN INITIAL` again.
- **Symbol table integration**: reuse your Assignment 1 symbol table as a header/class, strip out its `main()` and all console/file I/O, and instantiate one symbol-table object to call from the rules' actions.
- **Tricky escaping/edge cases called out in lecture**:
  - Character literal `'\n'` — you read `'`, `\`, `n` as 3 raw characters but must store/print it as the actual newline character (not literally "enter"), and log it back out as `\` and `n`.
  - Similarly handle `'\\'` correctly so it isn't mistaken for an unterminated literal.
  - Multi-character literal errors (e.g. `'abc'`) must not misfire on legitimate escapes like `'\n'`.
  - Strings can span multiple lines if a line ends with `\`; handle both `\n` and `\r\n` line endings.
  - Single-line comments starting with `//` also continue across lines if the line ends with `\`.

---

## 2. Problem Statement (What Needs to Be Done)

Build a **Flex-based lexical analyzer** for a C subset that reads a source file, produces two output files, and correctly detects tokens, symbol-table entries, line numbers, and lexical errors.

### 2.1 Token categories to recognize

| Category | Details |
|---|---|
| **Keywords** | `if, else, goto, for, while, long, do, break, short, int, char, static, float, double, unsigned, void, return, switch, case, default, continue` → printed as `<KEYWORD_TOKEN>` (e.g. `<IF>`). **Not** inserted into the symbol table. |
| **Integer constants** | One or more digits → `<CONST_INT, value>`, inserted into symbol table. |
| **Float constants** | Forms like `3.14159`, `3.14159E-10`, `.314159`, `000.0314`, `314159E10` → `<CONST_FLOAT, value>`, inserted into symbol table. |
| **Character constants** | Single quoted single character, or an escape sequence (`\n \t \\ \' \a \f \r \b \v \0`) → `<CONST_CHAR, value>` (store the actual ASCII value, not the raw quote/escape text), inserted into symbol table. |
| **Operators/punctuators** | See table below → `<TYPE, symbol>`. **Not** inserted into symbol table. `{` and `}` also trigger entering/exiting a symbol-table scope. |
| **Identifiers** | Start with letter or `_`, then alphanumerics/`_` → `<ID, name>`, inserted into symbol table. |
| **Strings** | Double-quoted, can be single- or multi-line (line continued with trailing `\`) → `<STRING, value>` with escape sequences resolved to actual characters. **Not** inserted into symbol table. |
| **Comments** | `//...` (single line, continuable with trailing `\`) and `/* ... */` (multi-line) → recognized and logged, but **no token emitted**. |
| **Whitespace** | Consumed silently, no token, no log action needed. |

**Operators/Punctuators table:**

| Symbols | Token Type |
|---|---|
| `+ -` | ADDOP |
| `* / %` | MULOP |
| `++ --` | INCOP |
| `< <= > >= == !=` | RELOP |
| `=` | ASSIGNOP |
| `&& \|\|` | LOGICOP |
| `!` | NOT |
| `(` | LPAREN |
| `)` | RPAREN |
| `{` | LCURL (enter new symbol-table scope) |
| `}` | RCURL (exit current symbol-table scope) |
| `[` | LTHIRD |
| `]` | RTHIRD |
| `,` | COMMA |
| `;` | SEMICOLON |

### 2.2 Line counting

Track and report the total number of lines in the source program (`yylineno`).

### 2.3 Lexical error detection

Detect and report (with line number) each of the following, and keep a running **total error count**:

1. Too many decimal points — e.g. `1.2.345.789`
2. Ill-formed number — e.g. `1E10.7`
3. Invalid suffix on a numeric constant / invalid prefix on an identifier — e.g. `12abcd`
4. Multi-character constant — e.g. `'abc'`
5. Unfinished character constant — e.g. `'a`, `'\n`, `'\`
6. Empty character constant — `''`
7. Unfinished string
8. Unfinished comment
9. Unrecognized character

### 2.4 Required outputs

Two files, named using your **7-digit student ID**:

1. **`<StudentID>_token.txt`** — all emitted tokens (in the format `<TYPE,lexeme>` or `<TYPE>` for keywords/operators with no attribute), in order.
2. **`<StudentID>_log.txt`** — a full trace of actions:
   - For every non-whitespace lexeme found:
     `Line No. <n>: Token <TOKEN> Lexeme <lexeme> found.`
     (this includes comments, even though comments produce no token in the token file)
   - For `CONST_CHAR` and `STRING`, also print the resolved token value in the log.
   - For every symbol-table insertion, print the **symbol table state** (only non-empty buckets) using the given hash function.
   - If a symbol already exists in the table, print an appropriate "already exists" message instead of re-inserting.
   - For every detected error:
     `Line No. <n>: <error message>.`
   - At the end of the log file: total line count and total error count.

### 2.5 Symbol table hashing requirement

Use the given `sdbmHash` function with **bucket size 7** so your symbol-table bucket layout matches the sample output:

```c
unsigned int sdbmHash(const char *p) {
    unsigned int hash = 0;
    auto *str = (unsigned char *) p;
    int c{};
    while ((c = *str++)) {
        hash = c + (hash << 6) + (hash << 16) - hash;
    }
    return hash;
}
```

### 2.6 Input/Output mechanics

- Input: a C source text file, filename given via the command line (lecture notes also mention a default of `input.txt` — confirm which is expected/whether both should be supported).
- Compare your output closely against the sample input/output provided on Moodle — matching the sample is explicitly encouraged.

### 2.7 Submission format

1. Create a folder named with your **7-digit student ID**.
2. Put your Flex source as `<student_id>.l`, plus any additional `.cpp`/`.h` files needed (e.g. the symbol table header). **Do not** include the generated `lex.yy.c` or any compiled executable.
3. Zip the folder as `<student_id>.zip`.
4. Submit via Moodle.
5. **Deadline:** Friday, May 24, 2026, 11:55 PM. Contact: `rumi@teacher.cse.buet.ac.bd`.

---

## 3. Implementation Steps

### Step 1 — Prepare the symbol table module
- [ ] Take your Assignment 1 symbol table implementation.
- [ ] Strip out its `main()` function and all console/file I/O.
- [ ] Refactor it so insert/lookup/scope-enter/scope-exit operations can be called from Flex actions and can **write to the log file** instead of stdout.
- [ ] Implement `sdbmHash` with bucket size 7 inside (or alongside) the symbol table so bucket printing matches the spec.
- [ ] Add functions to print only the **non-empty buckets** of the current scope/table.
- [ ] Save it as a header (`.h`) or a `.cpp` you can `#include` from the `.l` file.

### Step 2 — Set up the `.l` file skeleton
- [ ] Add `%option noyywrap` and enable `yylineno` (`%option yylineno` or set `yylineno = 1` before calling `yylex()`).
- [ ] In the `%{ ... %}` block: include your symbol table header, `<cstdio>`, `<string>`, etc.; declare global counters (`line_count`, `error_count`); declare/open the two output file streams (token file, log file); instantiate the symbol table object.
- [ ] In the definitions section, declare named subpatterns you'll reuse, e.g.:
  - `Letter [a-zA-Z_]`
  - `Digit [0-9]`
  - etc.
- [ ] Declare any start states you'll need with `%x` (exclusive), e.g. `%x COMMENT_STATE`, `%x STRING_STATE`.

### Step 3 — Write rules for whitespace and simple tokens
- [ ] Whitespace rule: match and ignore (increment nothing, print nothing — but let `yylineno` auto-increment on newlines if enabled, or increment manually).
- [ ] Keyword rules: one rule per keyword (or a single identifier-like rule that checks against a keyword set) — **write keyword rules before the generic identifier rule**, or use symbol-table/keyword-table lookup logic to disambiguate.
- [ ] Operator/punctuator rules: implement each row of the operator table above; be careful with **longest-match ordering** — e.g. `++`/`--` before `+`/`-`, `<=`,`>=`,`==`,`!=` before `<`,`>`,`=`,`!`.
- [ ] `{` and `}` rules: call symbol table's "enter new scope" / "exit scope" in addition to emitting `LCURL`/`RCURL`.

### Step 4 — Identifiers and numeric constants
- [ ] Identifier rule: `[a-zA-Z_][a-zA-Z0-9_]*` → check if it's a keyword first (if not handled via ordering); else emit `<ID, lexeme>` and insert into symbol table (or report "already exists").
- [ ] Integer rule: `{Digit}+` → emit `<CONST_INT, lexeme>`, insert into symbol table.
- [ ] Float rule(s): handle the forms `3.14159`, `.314159`, `000.0314`, `3.14159E-10`, `314159E10` — construct a regex (or a few regexes) covering optional leading digits, decimal point, optional exponent with sign. Emit `<CONST_FLOAT, lexeme>`, insert into symbol table.
- [ ] Error rule for **invalid suffix / bad identifier prefix**: pattern like digits immediately followed by letters (`{Digit}+{Letter}+...`) → report "Invalid suffix on numeric constant / prefix on identifier" and recover (don't crash the scan).
- [ ] Error rule for **too many decimal points**: numbers with 2+ `.` → report error.
- [ ] Error rule for **ill-formed number** (e.g., exponent followed by another decimal point like `1E10.7`) → report error.

### Step 5 — Character literals
- [ ] Rule for normal single character: `'` + one printable char (not `\` or `'`) + `'` → resolve to its ASCII value, emit `<CONST_CHAR, value>`, insert into symbol table, log the resolved value.
- [ ] Rule for escaped character: `'\` + one of `n t \\ ' a f r b v 0` + `'` → convert to the actual escape value (e.g. `\n` → newline char), emit token, insert into symbol table, but **log it back out as the two-character escape sequence** (e.g., print `\n` not an actual newline), per the lecture's guidance.
- [ ] Error: **empty character constant** `''`.
- [ ] Error: **multi-character constant** `'abc'` — but make sure this doesn't fire for legitimate escape sequences like `'\n'` or `'\\'` (use a start state or a carefully ordered/negated pattern).
- [ ] Error: **unfinished character constant** — `'a`, `'\n` (no closing quote), `'\` (dangling backslash) — likely needs a start state that you enter on seeing `'`, then match either a proper close or hit EOF/newline and report the error, then recover.

### Step 6 — String literals (likely needs a start state)
- [ ] On seeing `"`, `BEGIN` a `STRING_STATE`, start accumulating the literal.
- [ ] Inside the string state, resolve escape sequences (`\n`, `\t`, `\\`, `\"`, `\a`, `\b`, etc.) to their actual characters as you accumulate.
- [ ] Handle **line continuation**: if a line inside the string ends with a lone `\` immediately before the newline, keep accumulating on the next line rather than treating it as end-of-string or an error.
- [ ] Handle both `\n` and `\r\n` line endings when detecting continuations/newlines inside strings.
- [ ] On seeing the closing (unescaped) `"`, emit `<STRING, resolved_value>`, log it, `BEGIN INITIAL`.
- [ ] Error: **unfinished string** — reaching EOF or an unescaped newline (without a continuation `\`) while still inside `STRING_STATE`.

### Step 7 — Comments (needs a start state for the multi-line case)
- [ ] Single-line comment `//...`: match to end of line; if the line ends with `\`, continue matching into the next line (loop/consume until a line doesn't end with `\`); log as a `COMMENT` action but emit **no token**.
- [ ] Multi-line comment `/* ... */`: on `/*`, `BEGIN` a `COMMENT_STATE`; consume characters (tracking newlines for the line counter) until `*/`, then `BEGIN INITIAL`; log as `COMMENT`, no token.
- [ ] Error: **unfinished comment** — reaching EOF while still inside a multi-line comment state.

### Step 8 — Catch-all / unrecognized character
- [ ] Add a final fallback rule (typically `.`) matching any otherwise-unmatched character → report **"Unrecognized character"** error with line number and continue scanning (don't halt).

### Step 9 — Subroutine section / `main()`
- [ ] Parse the input filename from `argv` (per spec: from the command line) and open it, assigning to `yyin`.
- [ ] Derive/build the output filenames `<StudentID>_token.txt` and `<StudentID>_log.txt` and open them for writing.
- [ ] Initialize counters (`line_count = 1`, `error_count = 0`), set `yylineno = 1`.
- [ ] Call `yylex()`.
- [ ] After scanning completes, print final `line_count` and `error_count` to the log file.
- [ ] Close all files, clean up.

### Step 10 — Build and test
- [ ] Compile: `flex <student_id>.l` → generates `lex.yy.c`; compile that together with your symbol table `.cpp` (e.g. `g++ lex.yy.c symbol_table.cpp -o scanner -lfl` — adjust link flags as needed).
- [ ] Run against the sample input(s) from Moodle and **diff your output byte-for-byte** against the provided sample `_token.txt` / `_log.txt` files.
- [ ] Specifically test each error class individually (`1.2.345.789`, `1E10.7`, `12abcd`, `'abc'`, `'a`, `'\n` unterminated, `''`, unterminated string, unterminated comment, a genuinely unrecognized character like `` ` `` or `$` if not otherwise handled) to confirm each triggers the right message.
- [ ] Test edge cases explicitly flagged in the lecture: `'\n'` vs `'\\'` handling, multi-line strings with trailing `\`, single-line comments continued with trailing `\`, `\r\n` line endings, symbol table "already exists" messaging, and correct bucket printing (only non-empty buckets) using `sdbmHash` with 7 buckets.
- [ ] Verify rule ordering resolves ambiguities correctly (e.g., keywords not being caught as identifiers, multi-char operators not being split into single-char ones).

### Step 11 — Package for submission
- [ ] Create a folder named with your 7-digit student ID.
- [ ] Place `<student_id>.l` and any helper `.cpp`/`.h` files inside (no `lex.yy.c`, no binaries).
- [ ] Zip the folder as `<student_id>.zip` and submit on Moodle before the deadline.

---

## 4. Quick Reference Checklist

- [ ] All keyword tokens implemented and correctly prioritized over identifiers
- [ ] Integer, float, char constants all correctly tokenized and inserted into symbol table
- [ ] All operators/punctuators implemented with correct longest-match ordering
- [ ] `{`/`}` trigger scope enter/exit in the symbol table
- [ ] Identifiers tokenized and inserted (with "already exists" handling)
- [ ] Strings (single & multi-line) correctly tokenized with escapes resolved
- [ ] Comments (single & multi-line, with continuation) recognized, logged, no token emitted
- [ ] Whitespace consumed silently
- [ ] Line count tracked accurately
- [ ] All 9 error classes detected with correct messages and line numbers
- [ ] Error count tracked and printed at end of log
- [ ] Token file and log file named and formatted per spec
- [ ] Symbol table printed (non-empty buckets only) using `sdbmHash`, bucket size 7
- [ ] Output verified against Moodle sample files
- [ ] Submission zipped and named correctly with only required files
