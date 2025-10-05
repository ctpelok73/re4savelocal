@echo off
setlocal enabledelayedexpansion

echo Downloading dependencies for RE4 Save Redirect Plugin...
echo.

REM Store the original directory
set "ORIGINAL_DIR=%CD%"

REM Check if git is available
where git >nul 2>nul
if %errorlevel% neq 0 (
    echo Git is not installed or not in PATH.
    echo Please install Git from https://git-scm.com/
    echo.
    pause
    exit /b 1
)

REM Check if curl is available
where curl >nul 2>nul
if %errorlevel% neq 0 (
    echo curl is not available. Using PowerShell to download files.
)

REM Create dependencies directory in the same folder as the script
if not exist "%~dp0deps" mkdir "%~dp0deps"
cd /d "%~dp0deps"

REM Download Detours from GitHub
echo Downloading Microsoft Detours...
if exist "detours" (
    echo Detours directory already exists. Updating...
    cd detours
    git fetch
    git pull
    cd ..
) else (
    echo Cloning Detours repository...
    git clone https://github.com/microsoft/detours.git
)

echo.
echo Dependencies downloaded successfully!
echo.
echo Next steps:
echo 1. Install Visual Studio Community (needed to build Detours)
echo 2. Build Detours library manually:
echo    - Open Visual Studio Developer Command Prompt
echo    - Navigate to detours/src directory
echo    - Run "nmake"
echo 3. Copy detours.lib and detours.h to the project directory (where this script is located)
echo.
echo For detailed instructions, see MINGW_BUILD.md
echo.

REM Return to the original directory
cd /d "%ORIGINAL_DIR%"

pause