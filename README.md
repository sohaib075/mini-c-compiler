# Mini-C Compiler (FAST-NUCES CFD)

A professional 7-phase compiler for a subset of the C language, implemented using **Flex**, **Bison**, and **C**. This project was developed as the final requirement for the **Compiler Construction (CS-XXX)** course at FAST National University, Spring 2026.

---

## 🚀 Key Features
- **Full Pipeline:** Lexical analysis to x86 assembly and final program execution.
- **AST Generation:** High-fidelity Abstract Syntax Tree with Unicode visualization.
- **Semantic Safety:** Comprehensive type checking, scope validation, and undeclared variable detection.
- **TAC Optimizer:** Three optimization passes (Constant Folding, Copy Propagation, Dead Code Elimination).
- **Interactive Runner:** Menu-driven terminal interface for phase-by-phase testing.
- **Test Suite:** 20 verified test cases covering all language features and error conditions.

---

## 📂 Project Structure
```text
mini-c-compiler/
├── include/                # Header files (Interfaces)
│   └── *.h                 # All module definitions
├── src/                    # C Source Code (Implementation)
│   ├── main.c              # Core compiler driver
│   ├── ast.c               # AST implementation
│   ├── symtab.c            # Semantic analysis
│   ├── tac.c               # Intermediate code (TAC)
│   ├── optimizer.c         # TAC optimizations
│   ├── codegen.c           # x86 assembly generation
│   └── interpreter.c       # Virtual machine / execution
├── grammar/                # Grammar Specifications
│   ├── lexer.l             # Lexer specification (Flex)
│   └── parser.y            # Parser specification (Bison)
├── test/                   # Comprehensive Test Suite (20 cases)
├── output/                 # Generated Target Assembly
├── Mini_C_Compiler_Report.md # Formal Project Report (15+ pages)
├── runner.c                # Interactive Project Runner (utility)
├── Makefile                # Unified build system
├── START_RUNNER.bat        # Interactive runner entry point (Windows)
├── TEST_ALL.bat            # Full automated test suite (Windows)
├── CLEANUP.bat             # Reset environment for submission
└── README.md               # Project documentation
```

---

## 🛠️ Installation & Build

### Prerequisites
- **GCC** (GNU Compiler Collection)
- **Flex** (Lexical Analyzer Generator)
- **Bison** (Parser Generator)
- **Make** (optional, for Linux)

### Quick Start (Windows)
1. Double-click **`START_RUNNER.bat`**.
2. This will build the project and launch the **Interactive Project Runner**.
3. Use the menu to run specific phases or test cases.

### Manual Build (Linux/Cmd)
```bash
# Build the compiler and runner
make all

# Run a specific test
./minicc test/test1_simple.c

# Run with target phase (e.g., stop at Semantic Analysis)
./minicc test/test1_simple.c 3
```

---

## 🧪 Validation & Testing
The compiler has been validated against 20 test cases:
1. **Tests 1-14**: Feature validation (Branching, Loops, Functions, I/O).
2. **Tests 15-17**: Error handling (Syntax, Semantic, Duplicate declarations).
3. **Tests 18-20**: Edge cases and full integration.

---

## 📄 Project Report
A complete technical report detailing the grammar (EBNF), architecture, implementation details for all 7 phases, and challenges faced is available in:
**[Mini_C_Compiler_Report.md](Mini_C_Compiler_Report.md)**

---

## 👥 Authors
- **Muhammad Sohaib** - 22F-3302

**Instructor:** Dr. Hafiz Muhammad Tayyab Javeed  
**Semester:** Spring 2026 — 8th Semester, FAST National University, CFD Campus.
