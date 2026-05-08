@echo off
setlocal
cd /d "%~dp0"
echo ========================================
echo   Mini-C Compiler — Build and Run Runner
echo ========================================

:: Ensure correct MSYS2 UCRT64 environment (Fixes clock_gettime64 error)
if exist C:\msys64\ucrt64\bin (
    set "PATH=C:\msys64\ucrt64\bin;C:\msys64\usr\bin;%PATH%"
)

:: Check for GCC
gcc --version >nul 2>&1
if %errorlevel% neq 0 (
    echo [ERROR] GCC compiler not found! 
    echo Please install MSYS2 UCRT64 or MinGW-w64 and add it to your PATH.
    pause
    exit /b 1
)

:: Build the project
echo Building Compiler and Runner...
call make all

if %errorlevel% neq 0 (
    echo.
    echo [ERROR] Build Failed!
    echo Check your Makefile and MSYS2 environment.
    pause
    exit /b 1
)

echo.
echo Build Successful!
echo Starting Project Runner...
timeout /t 1 >nul
call runner.exe

endlocal
