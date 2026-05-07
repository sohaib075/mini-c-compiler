# ============================================================
#   MINI-C COMPILER — Build System (Makefile)
#   
#   Prerequisites:
#     - GCC (MinGW or similar on Windows)
#     - Flex (Win-flex on Windows)
#     - Bison (Win-bison on Windows)
#
#   Usage:
#     make          — Build the compiler
#     make clean    — Remove generated files
#     make test     — Run all test cases
#     make test1    — Run a specific test
# ============================================================

# ── Compiler and Tools ──
CC       = gcc
FLEX     = flex
BISON    = bison
CFLAGS   = -Wall -Wno-unused-function -g -Isrc
LDFLAGS  = 

# ── Directories ──
SRC_DIR  = src
OUT_DIR  = output
TEST_DIR = test

# ── Output binary ──
TARGET   = minicc

# ── Source files ──
BISON_SRC = $(SRC_DIR)/parser.y
FLEX_SRC  = $(SRC_DIR)/lexer.l

# Generated files from Flex/Bison
BISON_C   = $(SRC_DIR)/parser.tab.c
BISON_H   = $(SRC_DIR)/parser.tab.h
FLEX_C    = $(SRC_DIR)/lex.yy.c

# Hand-written C source files
C_SRCS   = $(SRC_DIR)/ast.c \
           $(SRC_DIR)/symtab.c \
           $(SRC_DIR)/tac.c \
           $(SRC_DIR)/optimizer.c \
           $(SRC_DIR)/codegen.c \
           $(SRC_DIR)/interpreter.c \
           $(SRC_DIR)/main.c

# All C sources (generated + hand-written)
ALL_SRCS = $(BISON_C) $(FLEX_C) $(C_SRCS)

# ════════════════════════════════════════════════
#  Build Rules
# ════════════════════════════════════════════════

# Default: build everything
all: dirs $(TARGET)
	@echo.
	@echo ========================================
	@echo   Mini-C Compiler built successfully!
	@echo   Run: $(TARGET).exe [input.c]
	@echo ========================================

# Create output directory
dirs:
	@if not exist $(OUT_DIR) mkdir $(OUT_DIR)

# Link everything into the final binary
$(TARGET): $(BISON_C) $(FLEX_C) $(C_SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(ALL_SRCS) $(LDFLAGS)

# Generate parser from Bison grammar
$(BISON_C) $(BISON_H): $(BISON_SRC)
	$(BISON) -d -o $(BISON_C) $(BISON_SRC)

# Generate lexer from Flex specification
$(FLEX_C): $(FLEX_SRC) $(BISON_H)
	$(FLEX) -o $(FLEX_C) $(FLEX_SRC)

# ════════════════════════════════════════════════
#  Test Rules
# ════════════════════════════════════════════════

test: all
	@echo.
	@echo ===== Running Test 1: Simple Assignment =====
	./$(TARGET) $(TEST_DIR)/test1_simple.c
	@echo.
	@echo ===== Running Test 2: Arithmetic =====
	./$(TARGET) $(TEST_DIR)/test2_arithmetic.c
	@echo.
	@echo ===== Running Test 3: If-Else =====
	./$(TARGET) $(TEST_DIR)/test3_if_else.c
	@echo.
	@echo ===== Running Test 4: While Loop =====
	./$(TARGET) $(TEST_DIR)/test4_while_loop.c
	@echo.
	@echo ===== Running Test 5: Functions =====
	./$(TARGET) $(TEST_DIR)/test5_functions.c
	@echo.
	@echo ===== Running Test 6: Comprehensive =====
	./$(TARGET) $(TEST_DIR)/test6_comprehensive.c

test1: all
	./$(TARGET) $(TEST_DIR)/test1_simple.c

test2: all
	./$(TARGET) $(TEST_DIR)/test2_arithmetic.c

test3: all
	./$(TARGET) $(TEST_DIR)/test3_if_else.c

test4: all
	./$(TARGET) $(TEST_DIR)/test4_while_loop.c

test5: all
	./$(TARGET) $(TEST_DIR)/test5_functions.c

test6: all
	./$(TARGET) $(TEST_DIR)/test6_comprehensive.c

# ════════════════════════════════════════════════
#  Cleanup
# ════════════════════════════════════════════════

clean:
	@if exist $(TARGET).exe del $(TARGET).exe
	@if exist $(BISON_C) del $(BISON_C)
	@if exist $(BISON_H) del $(BISON_H)
	@if exist $(FLEX_C) del $(FLEX_C)
	@if exist $(OUT_DIR)\output.asm del $(OUT_DIR)\output.asm
	@echo Cleaned.

.PHONY: all clean test dirs test1 test2 test3 test4 test5 test6
