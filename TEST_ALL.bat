@echo off
REM ============================================================
REM   MINI-C COMPILER - Comprehensive Test Suite
REM   Runs ALL 20 tests (valid, invalid, edge cases)
REM ============================================================

cd /d "D:\uni data\semester 8\CC Final Project"
set "PATH=C:\msys64\ucrt64\bin;C:\msys64\usr\bin;C:\Windows\System32;C:\Windows"

echo.
echo ============================================
echo   MINI-C COMPILER - Comprehensive Test Suite
echo ============================================
echo.

if not exist output mkdir output

echo [1/3] Running Bison...
bison -d -o src\parser.tab.c src\parser.y
if errorlevel 1 ( echo BISON FAILED & pause & exit /b 1 )

echo [2/3] Running Flex...
flex -o src\lex.yy.c src\lexer.l
if errorlevel 1 ( echo FLEX FAILED & pause & exit /b 1 )

echo [3/3] Compiling with GCC...
gcc -Wall -Wno-unused-function -Wno-unused-variable -Wno-format-truncation -Isrc -o minicc.exe src\parser.tab.c src\lex.yy.c src\ast.c src\symtab.c src\tac.c src\optimizer.c src\codegen.c src\interpreter.c src\main.c
if errorlevel 1 ( echo GCC FAILED & pause & exit /b 1 )

echo Build SUCCESSFUL!
echo.
echo ============================================
echo   RUNNING VALID INPUT TESTS (1-14)
echo ============================================

set PASS=0
set FAIL=0

REM --- Valid Tests ---
for %%T in (test1_simple test2_arithmetic test3_if_else test4_while_loop test5_functions test6_complex test7_for_loop test8_printf test9_nested test10_logical test11_multi_func test12_types test13_unary test14_empty_func) do (
    echo.
    echo ==================================================
    echo   TESTING: test\%%T.c
    echo ==================================================
    minicc.exe test\%%T.c > nul 2>&1
    if errorlevel 1 (
        echo   RESULT: FAIL [exit code !errorlevel!]
        set /a FAIL+=1
        echo   --- Running again with output ---
        minicc.exe test\%%T.c
    ) else (
        echo   RESULT: PASS
        set /a PASS+=1
    )
)

echo.
echo ============================================
echo   RUNNING ERROR DETECTION TESTS (15-17)
echo   (These SHOULD show errors)
echo ============================================

echo.
echo ==================================================
echo   TEST 15: Syntax Error (missing semicolon)
echo   EXPECTED: Syntax error detected, compilation stops
echo ==================================================
minicc.exe test\test15_syntax_err.c 2>&1 | findstr /I "error"
if errorlevel 1 (
    echo   RESULT: FAIL - No error detected!
    set /a FAIL+=1
) else (
    echo   RESULT: PASS - Error correctly detected
    set /a PASS+=1
)

echo.
echo ==================================================
echo   TEST 16: Semantic Error (undeclared variable)
echo   EXPECTED: Undeclared variable error
echo ==================================================
minicc.exe test\test16_undeclared.c 2>&1 | findstr /I "Undeclared"
if errorlevel 1 (
    echo   RESULT: FAIL - Undeclared variable not detected!
    set /a FAIL+=1
) else (
    echo   RESULT: PASS - Undeclared variable detected
    set /a PASS+=1
)

echo.
echo ==================================================
echo   TEST 17: Semantic Error (duplicate declaration)
echo   EXPECTED: Duplicate declaration error
echo ==================================================
minicc.exe test\test17_duplicate.c 2>&1 | findstr /I "Duplicate"
if errorlevel 1 (
    echo   RESULT: FAIL - Duplicate not detected!
    set /a FAIL+=1
) else (
    echo   RESULT: PASS - Duplicate declaration detected
    set /a PASS+=1
)

echo.
echo ============================================
echo   RUNNING EDGE CASE TESTS (18-20)
echo ============================================

for %%T in (test18_precedence test19_comments test20_full) do (
    echo.
    echo ==================================================
    echo   TESTING: test\%%T.c
    echo ==================================================
    minicc.exe test\%%T.c > nul 2>&1
    if errorlevel 1 (
        echo   RESULT: FAIL
        set /a FAIL+=1
        echo   --- Running again with output ---
        minicc.exe test\%%T.c
    ) else (
        echo   RESULT: PASS
        set /a PASS+=1
    )
)

echo.
echo ============================================
echo   FINAL TEST RESULTS
echo ============================================
echo   Passed: %PASS%
echo   Failed: %FAIL%
echo ============================================
echo.
pause
