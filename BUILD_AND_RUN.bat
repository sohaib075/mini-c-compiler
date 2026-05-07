@echo off
REM ============================================================
REM   MINI-C COMPILER - Build and Run ALL Tests
REM   Double-click this file from File Explorer to build & run
REM ============================================================

cd /d "D:\uni data\semester 8\CC Final Project"

REM Fix PATH: Use working GCC installation
set "PATH=C:\msys64\ucrt64\bin;C:\msys64\usr\bin;C:\Windows\System32;C:\Windows"

echo.
echo ============================================
echo   MINI-C COMPILER - Build and Run
echo ============================================
echo.

REM Verify tools
echo Checking tools...
where gcc >nul 2>&1
if errorlevel 1 (
    echo ERROR: gcc not found!
    pause
    exit /b 1
)
gcc --version 2>&1 | findstr /C:"gcc"
echo.

if not exist output mkdir output

echo [1/3] Running Bison on parser.y...
bison -d -o src\parser.tab.c src\parser.y
if errorlevel 1 (
    echo FAILED: Bison error!
    pause
    exit /b 1
)
echo       Done.

echo [2/3] Running Flex on lexer.l...
flex -o src\lex.yy.c src\lexer.l
if errorlevel 1 (
    echo FAILED: Flex error!
    pause
    exit /b 1
)
echo       Done.

echo [3/3] Compiling with GCC...
gcc -Wall -Wno-unused-function -Wno-format-truncation -Isrc -o minicc.exe src\parser.tab.c src\lex.yy.c src\ast.c src\symtab.c src\tac.c src\optimizer.c src\codegen.c src\interpreter.c src\main.c
if errorlevel 1 (
    echo.
    echo FAILED: Compilation error!
    pause
    exit /b 1
)
echo       Done.

echo.
echo ============================================
echo   BUILD SUCCESSFUL! Running all tests...
echo ============================================

echo.
echo ========== TEST 1: Simple Variables ==========
minicc.exe test\test1_simple.c

echo.
echo ========== TEST 2: Arithmetic ==========
minicc.exe test\test2_arithmetic.c

echo.
echo ========== TEST 3: If/Else ==========
minicc.exe test\test3_if_else.c

echo.
echo ========== TEST 4: While Loop ==========
minicc.exe test\test4_while_loop.c

echo.
echo ========== TEST 5: Functions ==========
minicc.exe test\test5_functions.c

echo.
echo ========== TEST 6: Complex ==========
minicc.exe test\test6_complex.c

echo.
echo ============================================
echo   ALL TESTS COMPLETED
echo ============================================
pause
