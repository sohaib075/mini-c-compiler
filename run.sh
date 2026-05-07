#!/bin/bash
# ============================================================
#   MINI-C COMPILER - Build and Run (Linux/Kali)
#   Usage: chmod +x run.sh && ./run.sh
# ============================================================

echo ""
echo "============================================"
echo "  MINI-C COMPILER - Build and Run (Linux)"
echo "============================================"
echo ""

# Check tools
echo "Checking tools..."
for tool in gcc flex bison; do
    if ! command -v $tool &> /dev/null; then
        echo "ERROR: $tool not found!"
        echo "Install: sudo apt install gcc flex bison"
        exit 1
    fi
done
gcc --version | head -1
echo ""

# Create output dir
mkdir -p output

# Build
echo "[1/3] Running Bison..."
bison -d -o src/parser.tab.c src/parser.y || { echo "BISON FAILED!"; exit 1; }
echo "      Done."

echo "[2/3] Running Flex..."
flex -o src/lex.yy.c src/lexer.l || { echo "FLEX FAILED!"; exit 1; }
echo "      Done."

echo "[3/3] Compiling with GCC..."
gcc -Wall -Wno-unused-function -Wno-unused-variable -Wno-format-truncation \
    -Isrc -o minicc \
    src/parser.tab.c src/lex.yy.c src/ast.c src/symtab.c \
    src/tac.c src/optimizer.c src/codegen.c src/interpreter.c \
    src/main.c || { echo "GCC FAILED!"; exit 1; }
echo "      Done."

echo ""
echo "============================================"
echo "  BUILD SUCCESSFUL! Running test..."
echo "============================================"
echo ""

# Run with argument or default test
if [ -n "$1" ]; then
    ./minicc "$1"
else
    ./minicc test/test1_simple.c
fi
