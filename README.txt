============================================================
  MINI-C COMPILER — README
  FAST National University, CFD Campus
  Department of Computer Science
  Compiler Construction Project
============================================================

1. PROJECT DESCRIPTION
──────────────────────
This project implements a Mini-C Language Compiler that
demonstrates all six standard compiler phases with visible
output for each phase:

  Phase 1: Lexical Analysis     (Flex tokenizer)
  Phase 2: Syntax Analysis      (Bison parser + AST)
  Phase 3: Semantic Analysis    (Symbol table + type checking)
  Phase 4: Intermediate Code    (Three-Address Code generation)
  Phase 5: Code Optimization    (Constant folding + DCE)
  Phase 6: Code Generation      (x86 assembly output)


2. PREREQUISITES
──────────────────────
  - GCC compiler (MinGW-w64 on Windows)
  - Flex (lexical analyzer generator)
  - Bison (parser generator)
  - Make (GNU Make or nmake)

  Installation on Windows:
    1. Install MSYS2 from https://www.msys2.org/
    2. Open MSYS2 terminal and run:
       pacman -S mingw-w64-x86_64-gcc flex bison make
    3. Add MSYS2/mingw64/bin to your PATH


3. COMPILATION & BUILD
──────────────────────

  Option A: Using Makefile
    make              (builds the compiler)
    make clean        (removes generated files)

  Option B: Using build script (Windows)
    build.bat         (runs all build steps)

  Option C: Manual build steps
    1. bison -d -o src/parser.tab.c src/parser.y
    2. flex -o src/lex.yy.c src/lexer.l
    3. gcc -Wall -g -Isrc -o minicc src/parser.tab.c src/lex.yy.c src/ast.c src/symtab.c src/tac.c src/optimizer.c src/codegen.c src/main.c


4. RUNNING THE COMPILER
──────────────────────

  minicc.exe test\test1_simple.c

  This will process the input Mini-C file through all 6
  phases and display formatted output for each phase.

  The generated x86 assembly is saved to output\output.asm.


5. TEST FILES
──────────────────────
  test\test1_simple.c          Simple variable assignment
  test\test2_arithmetic.c      Arithmetic expressions
  test\test3_if_else.c         If-else control flow
  test\test4_while_loop.c      While loop
  test\test5_functions.c       Function declaration & calls
  test\test6_comprehensive.c   All features combined


6. PROJECT STRUCTURE
──────────────────────
  CC Final Project/
  ├── Makefile
  ├── build.bat
  ├── README.txt
  ├── src/
  │   ├── lexer.l          Phase 1: Flex lexer specification
  │   ├── parser.y         Phase 2: Bison grammar
  │   ├── ast.h / ast.c    AST node definitions & printing
  │   ├── symtab.h / .c    Phase 3: Symbol table & semantics
  │   ├── tac.h / tac.c    Phase 4: TAC generation
  │   ├── optimizer.h/.c   Phase 5: Code optimization
  │   ├── codegen.h / .c   Phase 6: x86 assembly generation
  │   └── main.c           Main driver
  ├── test/                Sample .c input files
  └── output/              Generated assembly output


7. SUPPORTED MINI-C FEATURES
──────────────────────
  - Data types: int, float, char
  - Variables: declaration and assignment
  - Arithmetic: + - * / %
  - Relational: == != < > <= >=
  - Logical: && || !
  - Control: if, else, while, for
  - Functions: declaration, parameters, return
  - I/O: printf with format specifiers
  - Comments: // single-line and /* multi-line */


============================================================
