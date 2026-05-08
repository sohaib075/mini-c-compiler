---

<p align="center" style="font-size:28px; font-weight:bold;">FAST National University of Computer and Emerging Sciences</p>
<p align="center" style="font-size:22px;">Chiniot-Faisalabad (CFD) Campus</p>
<p align="center" style="font-size:20px;">Department of Computer Science</p>

<br>

<p align="center" style="font-size:26px; font-weight:bold;">Compiler Construction — Final Project Report</p>
<p align="center" style="font-size:24px; font-weight:bold;">Mini-C Compiler</p>
<p align="center" style="font-size:16px;">A Complete 7-Phase Compiler for a Subset of the C Language</p>

<br><br>

<p align="center" style="font-size:18px; font-weight:bold;">Group Members</p>

| Name | Roll Number |
|------|-------------|
| [Student Name 1] | [Roll No 1] |
| [Student Name 2] | [Roll No 2] |
| [Student Name 3] | [Roll No 3] |

<br>

<p align="center"><b>Course:</b> Compiler Construction (CS-XXX)</p>
<p align="center"><b>Instructor:</b> [Instructor Name]</p>
<p align="center"><b>Semester:</b> Spring 2026 — 8th Semester</p>
<p align="center"><b>Submission Date:</b> May 2026</p>

---

<div style="page-break-after: always;"></div>

# Table of Contents

1. [Introduction & Problem Statement](#1-introduction--problem-statement)
2. [Grammar Specification (BNF / EBNF)](#2-grammar-specification-bnf--ebnf)
3. [System Architecture & Pipeline Overview](#3-system-architecture--pipeline-overview)
4. [Phase 1 — Lexical Analysis](#4-phase-1--lexical-analysis)
5. [Phase 2 — Syntax Analysis (Parsing & AST)](#5-phase-2--syntax-analysis-parsing--ast)
6. [Phase 3 — Semantic Analysis](#6-phase-3--semantic-analysis)
7. [Phase 4 — Intermediate Code Generation (TAC)](#7-phase-4--intermediate-code-generation-tac)
8. [Phase 5 — Code Optimization](#8-phase-5--code-optimization)
9. [Phase 6 — Target Code Generation (x86 Assembly)](#9-phase-6--target-code-generation-x86-assembly)
10. [Phase 7 — Program Execution (TAC Interpreter)](#10-phase-7--program-execution-tac-interpreter)
11. [Screenshots of Output for Each Phase](#11-screenshots-of-output-for-each-phase)
12. [Test Suite & Validation](#12-test-suite--validation)
13. [Challenges Faced & Resolutions](#13-challenges-faced--resolutions)
14. [Tools & Technologies](#14-tools--technologies)
15. [Conclusion](#15-conclusion)
16. [References](#16-references)

---

<div style="page-break-after: always;"></div>

# 1. Introduction & Problem Statement

## 1.1 Introduction
The construction of a compiler is one of the most complex tasks in computer science, requiring a deep understanding of formal languages, data structures, and computer architecture. This project involves the design and implementation of a **Mini-C Compiler**, a software system that translates source code written in a subset of the C programming language into executable machine-level logic.

The Mini-C Compiler is designed as a multi-stage pipeline, starting from raw text and progressing through lexical, syntactic, and semantic levels to an intermediate representation, which is then optimized and finally translated into target x86-style assembly code. For immediate verification, the project also includes a virtual execution environment in the form of a Three-Address Code (TAC) interpreter.

## 1.2 Problem Statement
The primary goal was to create a functional compiler for a "Mini-C" language that captures the essential features of C while remaining manageable for a single-semester project. The compiler must handle:
- **Variable Declarations:** Support for basic types (`int`, `float`, `char`).
- **Control Flow:** Standard branching (`if-else`) and looping (`while`, `for`) constructs.
- **Functions:** Support for function definition, parameter passing, and return values.
- **I/O Operations:** Integration of a `printf` function for formatted output.
- **Validation:** Detection and reporting of errors at different stages (Lexical, Syntax, Semantic).
- **Optimization:** Improving the efficiency of generated code through standard optimization techniques.

## 1.3 Scope of the Mini-C Language
The supported subset includes:
- **Types:** `int`, `float`, `char`, `void`.
- **Operators:** Arithmetic (`+`, `-`, `*`, `/`, `%`), Relational (`==`, `!=`, `<`, `>`, `<=`, `>=`), Logical (`&&`, `||`, `!`), and Assignment (`=`).
- **Constructs:** `if`, `else`, `while`, `for`, `return`, `printf`.
- **Functions:** Local and global scope, multiple parameters.

---

<div style="page-break-after: always;"></div>

# 2. Grammar Specification (BNF / EBNF)

The formal structure of the Mini-C language is defined using EBNF (Extended Backus-Naur Form). This grammar defines how tokens are combined into valid language constructs.

## 2.1 Top-Level Structure
```ebnf
<program>           ::= <declaration_list>
<declaration_list>  ::= <declaration> | <declaration_list> <declaration>
<declaration>       ::= <var_declaration> | <fun_declaration>
```

## 2.2 Declarations
```ebnf
<var_declaration>   ::= <type_specifier> IDENTIFIER ";"
                      | <type_specifier> IDENTIFIER "=" <expression> ";"

<type_specifier>    ::= "int" | "float" | "char" | "void"

<fun_declaration>   ::= <type_specifier> IDENTIFIER "(" <param_list> ")" <compound_stmt>
                      | <type_specifier> IDENTIFIER "(" ")" <compound_stmt>

<param_list>        ::= <param> | <param_list> "," <param>
<param>             ::= <type_specifier> IDENTIFIER
```

## 2.3 Statements
```ebnf
<compound_stmt>     ::= "{" <statement_list> "}" | "{" "}"
<statement_list>    ::= <statement> | <statement_list> <statement>
<statement>         ::= <var_declaration> | <expression_stmt> | <compound_stmt>
                      | <selection_stmt> | <iteration_stmt> | <return_stmt> | <printf_stmt>

<selection_stmt>    ::= "if" "(" <expression> ")" <statement>
                      | "if" "(" <expression> ")" <statement> "else" <statement>

<iteration_stmt>    ::= "while" "(" <expression> ")" <statement>
                      | "for" "(" <expression_stmt> <expression_stmt> <expression> ")" <statement>
```

## 2.4 Expressions and Precedence
The grammar enforces operator precedence by nesting rules:
1. **Assignment** (Lowest)
2. **Logical OR** (`||`)
3. **Logical AND** (`&&`)
4. **Equality** (`==`, `!=`)
5. **Relational** (`<`, `>`, `<=`, `>=`)
6. **Additive** (`+`, `-`)
7. **Multiplicative** (`*`, `/`, `%`)
8. **Unary** (`-`, `!`) (Highest)

---

<div style="page-break-after: always;"></div>

# 3. System Architecture & Pipeline Overview

The compiler is structured as a 7-phase linear pipeline. Each phase transforms the program into a progressively lower-level representation.

## 3.1 Pipeline Diagram
1. **Source Code (.c)**: Input Mini-C file.
2. **Lexical Analysis (Flex)**: Generates a stream of tokens.
3. **Syntax Analysis (Bison)**: Verifies grammar and builds an **Abstract Syntax Tree (AST)**.
4. **Semantic Analysis**: Checks symbol table for types, scope, and variable existence.
5. **Intermediate Code Generation (TAC)**: Converts AST to **Three-Address Code**.
6. **Code Optimization**: Constant folding and dead code elimination on TAC.
7. **Target Code Generation**: Translates optimized TAC to **x86 Assembly**.
8. **Execution (Interpreter)**: Runs the TAC directly to show final output.

---

<div style="page-break-after: always;"></div>

# 4. Phase 1 — Lexical Analysis

## 4.1 Overview
The lexical analyzer, implemented using **Flex**, is responsible for scanning the input characters and grouping them into tokens. It filters out comments and whitespace and identifies keywords, identifiers, literals, and operators.

## 4.2 Implementation Details
- **Keywords:** `int`, `float`, `if`, `else`, `while`, `for`, `return`, etc.
- **Identifiers:** `[a-zA-Z_][a-zA-Z0-9_]*`
- **Numbers:** `[0-9]+` (integer), `[0-9]+"."[0-9]+` (float)
- **Token Table:** The lexer maintains a record of every token's value, type, and line number for error reporting and debugging.

## 4.3 Error Handling
If an unrecognized character is encountered, the lexer reports a "Lexical Error" with the specific character and line number, preventing further compilation if the error is fatal.

---

<div style="page-break-after: always;"></div>

# 5. Phase 2 — Syntax Analysis (Parsing & AST)

## 5.1 Overview
The syntax analyzer is implemented with **Bison**. It takes the token stream from Phase 1 and validates it against the Mini-C grammar.

## 5.2 Abstract Syntax Tree (AST)
Rather than just checking for syntax correctness, our parser builds a comprehensive **Abstract Syntax Tree (AST)**. This tree captures the hierarchical structure of the program:
- **Program Node:** The root of the tree.
- **Function/Variable Decl Nodes:** Representing declarations.
- **Statement Nodes:** Representing loops, if-statements, etc.
- **Expression Nodes:** Representing arithmetic and logical operations.

The AST is the primary data structure passed to all subsequent phases.

---

<div style="page-break-after: always;"></div>

# 6. Phase 3 — Semantic Analysis

## 6.1 Overview
Semantic analysis ensures the program "makes sense" beyond just grammar rules. This is achieved using a **Symbol Table**.

## 6.2 Symbol Table Management
The symbol table stores:
- Variable and function names.
- Data types (`int`, `float`, etc.).
- Scope information (local vs. global).
- Line numbers of declaration.

## 6.3 Semantic Checks
- **Variable Declaration:** Ensures variables are declared before use.
- **Scope Validation:** Ensures local variables are not accessed outside their block.
- **Type Checking:** Ensures operations are performed on compatible types (e.g., preventing arithmetic on string literals).
- **Function Parameters:** Checks if the number of arguments in a call matches the definition.

---

<div style="page-break-after: always;"></div>

# 7. Phase 4 — Intermediate Code Generation (TAC)

## 7.1 Overview
The AST is traversed recursively to generate **Three-Address Code (TAC)**. TAC is a language-independent intermediate representation where each instruction involves at most three operands.

## 7.2 Instruction Set
- **Assignment:** `x = y op z` or `x = y`
- **Control Flow:** `goto L1`, `ifFalse t0 goto L2`
- **Function Calls:** `param x`, `call func, n`
- **I/O:** `print_int t0`, `print_str "hello"`

TAC allows the compiler to perform optimizations without being tied to a specific machine architecture.

---

<div style="page-break-after: always;"></div>

# 8. Phase 5 — Code Optimization

## 8.1 Overview
The generated TAC is often redundant. The optimizer performs three passes to improve efficiency:

## 8.2 Optimization Passes
1. **Constant Folding:** Expressions like `5 + 3` are evaluated at compile-time to `8`.
2. **Copy Propagation:** If `x = y` and `z = x + 1`, it is transformed to `z = y + 1` to eliminate unnecessary temporaries.
3. **Dead Code Elimination:** Removes instructions whose results are never used.

These passes run in a loop until no more changes can be made (fixed-point iteration).

---

<div style="page-break-after: always;"></div>

# 9. Phase 6 — Target Code Generation (x86 Assembly)

## 9.1 Overview
This phase translates the optimized TAC into x86 assembly code. This code is designed to be compatible with assemblers like NASM or directly callable from C.

## 9.2 Mapping to Assembly
- **Data Section:** All variables and temporary registers are declared in the `.data` segment.
- **Text Section:** Contains the logic.
- **Register Usage:** `EAX`, `EBX`, `ECX` are used for arithmetic operations.
- **Control Flow:** `goto` translates to `jmp`, and labels are kept as assembly labels.

---

<div style="page-break-after: always;"></div>

# 10. Phase 7 — Program Execution (TAC Interpreter)

## 10.1 Overview
The final phase is the **TAC Interpreter**. This is a virtual machine that "executes" the TAC instructions.

## 10.2 Functionality
- **Memory Simulation:** A hash map or array simulates variable storage.
- **Execution Loop:** Iterates through TAC instructions, updating the virtual state.
- **Standard I/O:** The interpreter handles `print` instructions by calling actual C `printf` functions.

This phase provides a convenient way to verify the compiler's correctness without needing an external assembler or linker.

---

<div style="page-break-after: always;"></div>

# 11. Screenshots of Output for Each Phase

The following screenshots demonstrate the compiler processing a comprehensive test case (`test6_complex.c`).

### 11.1 Phase 1: Lexical Analysis Output
A table showing tokens, their types, and line numbers.

*(Placeholder: Insert screenshot of Token Table)*

### 11.2 Phase 2: AST Visualization
An indented tree structure representing the parsed code.

*(Placeholder: Insert screenshot of indented AST)*

### 11.3 Phase 3: Symbol Table
A list of all identified symbols and their properties.

*(Placeholder: Insert screenshot of Symbol Table)*

### 11.4 Phase 4 & 5: TAC and Optimized TAC
The raw intermediate code versus the code after constant folding and dead code elimination.

*(Placeholder: Insert screenshot of TAC comparison)*

### 11.5 Phase 6: x86 Assembly
The final generated assembly file.

*(Placeholder: Insert screenshot of output/output.asm)*

### 11.6 Phase 7: Final Execution
The output of the program as seen in the console.

*(Placeholder: Insert screenshot of final execution output)*

---

<div style="page-break-after: always;"></div>

# 12. Test Suite & Validation

The compiler was validated using a suite of 20 test cases.

| Test ID | Category | Description |
|---------|----------|-------------|
| 1-5 | Simple | Basic arithmetic, if-else, while loops. |
| 6-10 | Functional | Multiple functions, recursion, parameter passing. |
| 11-15 | Edge Cases | Nested loops, complex precedence, empty functions. |
| 16-20 | Error Handling | Undeclared variables, syntax errors, duplicate declarations. |

---

<div style="page-break-after: always;"></div>

# 13. Challenges Faced & Resolutions

### 13.1 Dangling-Else Ambiguity
- **Challenge:** Nested if-statements caused ambiguity in the parser.
- **Resolution:** Used Bison's `%nonassoc` and precedence rules to bind `else` to the closest `if`.

### 13.2 Memory Management
- **Challenge:** Building an AST for large programs consumed significant memory.
- **Resolution:** Implemented recursive `free_ast` functions to ensure all nodes are cleaned up after use.

### 13.3 TAC to Assembly Mapping
- **Challenge:** Mapping temporary TAC variables to physical registers or memory.
- **Resolution:** Implemented a memory-based approach where all TAC temporaries are assigned unique names in the `.data` segment of the assembly file.

---

<div style="page-break-after: always;"></div>

# 14. Tools & Technologies
- **Flex:** For lexical scanning.
- **Bison:** For LALR parsing.
- **C Language:** Primary language for compiler implementation.
- **MinGW/GCC:** For compilation of the compiler itself.
- **x86 Pseudo-Assembly:** Target language.

---

<div style="page-break-after: always;"></div>

# 15. Conclusion
This project demonstrates the complete lifecycle of source code translation. By implementing all seven phases, we gained deep insights into how high-level abstractions in C are converted into low-level machine logic. The resulting Mini-C Compiler is a robust academic tool capable of compiling and executing functional C-like programs with optimizations.

---

<div style="page-break-after: always;"></div>

# 16. References
1. Aho, Sethi, Ullman - *Compilers: Principles, Techniques, and Tools* (The Dragon Book).
2. Flex & Bison Documentation.
3. Intel x86 Instruction Set Reference.
4. FAST-NUCES Compiler Construction Course Materials.

---
