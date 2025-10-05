@echo off
echo Building RE4 Save Redirect Plugin with MinGW...
echo.

REM Check if MinGW is available
where g++ >nul 2>nul
if %errorlevel% neq 0 (
    echo MinGW-w64 not found in PATH.
    echo.
    echo Please install MinGW-w64 (32-bit) and make sure g++ is in your PATH.
    echo You can download it from: https://www.mingw-w64.org/downloads/
    echo.
    pause
    exit /b 1
)

echo Compiling the plugin...
g++ -shared -O2 -Wall -Wextra -D_WIN32_WINNT=0x0501 dllmain.cpp -o RE4SaveRedirect.dll -L. -ldetours -lshlwapi

if %errorlevel% equ 0 (
    echo.
    echo Build completed successfully!
    echo The plugin file is: RE4SaveRedirect.dll
    echo.
    echo You can now copy this DLL to your RE4 game directory.
) else (
    echo.
    echo Build failed!
    echo Make sure you have MinGW-w64 installed and detours library available.
)

pause