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

REM Check if detours library exists
if exist "detours.lib" (
    echo Found detours.lib. Building with real Detours library...
    g++ -shared -O2 -Wall -Wextra -D_WIN32_WINNT=0x0501 dllmain.cpp -o RE4SaveRedirect.dll -L. -ldetours -lshlwapi
) else (
    echo detours.lib not found. Building with stubs for testing purposes...
    echo Note: The resulting DLL will not function without the real Detours library.
    g++ -shared -O2 -Wall -Wextra -D_WIN32_WINNT=0x0501 -D_USE_DETOURS_STUB dllmain.cpp -o RE4SaveRedirect.dll -lshlwapi
)

if %errorlevel% equ 0 (
    echo.
    echo Build completed successfully!
    echo The plugin file is: RE4SaveRedirect.dll
    echo.
    if not exist "detours.lib" (
        echo NOTE: This build uses stubs and will not function without the real Detours library.
        echo For a working plugin, install the Microsoft Detours library.
    )
    echo You can now copy this DLL to your RE4 game directory.
) else (
    echo.
    echo Build failed!
    echo Make sure you have MinGW-w64 installed.
    if not exist "detours.lib" (
        echo For a functional plugin, you also need the Microsoft Detours library.
    )
)

pause