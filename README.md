# Mini-C Compiler

A complete 7-phase compiler for a subset of the C language, built with **Flex**, **Bison**, and **C**.  
Developed as a Compiler Construction course project at **FAST National University — CFD Campus**.

---

## Compiler Phases

| Phase | Description | Key Files |
|-------|-------------|-----------|
| **1. Lexical Analysis** | Tokenizes source code using Flex | `src/lexer.l` |
| **2. Syntax Analysis** | Parses tokens and builds an AST using Bison | `src/parser.y`, `src/ast.c` |
| **3. Semantic Analysis** | Symbol table, type checking, scope validation | `src/symtab.c` |
| **4. TAC Generation** | Converts AST to Three-Address Code | `src/tac.c` |
| **5. Code Optimization** | Constant folding, copy propagation, dead code elimination | `src/optimizer.c` |
| **6. Code Generation** | Translates TAC to x86-style assembly | `src/codegen.c` |
| **7. Program Execution** | Interprets optimized TAC and runs the program | `src/interpreter.c` |

---

## Project Structure

```
mini-c-compiler/
├── src/
│   ├── lexer.l            # Flex lexer specification
│   ├── parser.y           # Bison grammar specification
│   ├── ast.c / ast.h      # Abstract Syntax Tree
│   ├── symtab.c / symtab.h # Symbol table & semantic analysis
│   ├── tac.c / tac.h      # Three-Address Code generator
│   ├── optimizer.c / .h   # TAC optimizer (3 passes)
│   ├── codegen.c / .h     # x86 assembly generator
│   ├── interpreter.c / .h # TAC interpreter (execution)
│   └── main.c             # Compiler driver
├── test/                   # 20 test cases
├── output/                 # Generated assembly output
├── Makefile                # Linux build
├── BUILD_AND_RUN.bat       # Windows build
├── TEST_ALL.bat            # Windows test suite
└── run.sh                  # Linux build script
```

---

## Prerequisites

- **GCC** (any version)
- **Flex** (lexer generator)
- **Bison** (parser generator)

### Install on Linux (Kali/Ubuntu)
```bash
sudo apt install gcc flex bison make -y
```

### Install on Windows
Install [MSYS2](https://www.msys2.org/) and run:
```bash
pacman -S mingw-w64-ucrt-x86_64-gcc flex bison
```

---

## Build & Run

### Linux
```bash
make
./minicc test/test1_simple.c
```

### Windows
```
Double-click BUILD_AND_RUN.bat
```
Or in PowerShell:
```powershell
.\minicc.exe test\test1_simple.c
```

---

## Run All Tests

### Linux
```bash
make testall
```

### Windows
```
Double-click TEST_ALL.bat
```

**Test Suite:** 20 cases covering valid programs, syntax errors, semantic errors, and edge cases.

| Tests 1-14 | Valid inputs (variables, arithmetic, if/else, loops, functions, printf, types, unary ops) |
|------------|-------------------------|
| Tests 15-17 | Error detection (missing semicolon, undeclared variable, duplicate declaration) |
| Tests 18-20 | Edge cases (operator precedence, comments, full program) |

---

## Sample Output

For input `test/test1_simple.c`:
```c
int main() {
    int x = 5 + 3;
    int y = 10;
    int z = x + y;
    return 0;
}
```

The compiler produces all 7 phases:
- **Phase 1:** Token stream table
- **Phase 2:** Abstract Syntax Tree
- **Phase 3:** Symbol table
- **Phase 4:** Three-Address Code (12 instructions)
- **Phase 5:** Optimized TAC (7 instructions) with constant folding
- **Phase 6:** x86 assembly → saved to `output/output.asm`
- **Phase 7:** Program execution output

---

## Supported Language Features

- **Types:** `int`, `float`, `char`, `void`
- **Control Flow:** `if`, `else`, `while`, `for`
- **Functions:** Declaration, parameters, return values, recursive calls
- **Operators:** `+`, `-`, `*`, `/`, `%`, `==`, `!=`, `<`, `>`, `<=`, `>=`, `&&`, `||`, `!`
- **I/O:** `printf()` with `%d`, `%c`, `%s`, `\n`
- **Other:** Comments (`//`, `/* */`), `#include` directives, unary operators

---

## Tools & Technologies

- **Flex** — Lexical analysis
- **Bison** — Syntax analysis (LALR parsing)
- **C (GCC)** — All compiler phases
- **x86 Assembly** — Target code output

---

## License

This project was developed for academic purposes at FAST National University.
