# RE4 Save Redirect

A plugin that redirects where Resident Evil 4 (2005) saves game data to a custom location of your choice.

## Features

- Redirects RE4 save files to a custom directory
- Configurable via INI file
- Transparent operation - game functions normally
- Preserves all save data functionality

## Installation

1. Place `RE4SaveRedirect.dll` in the same directory as `re4.exe`
2. Edit `re4_save_redirect.ini` to specify your desired save directory
3. Install [Ultimate ASI Loader](https://github.com/ThirteenAG/Ultimate-ASI-Loader) in your RE4 game folder
4. Launch the game normally

## Configuration

Edit `re4_save_redirect.ini` to set your custom save path:

```ini
[Settings]
; Path where Resident Evil 4 save files will be redirected
; Use forward slashes or double backslashes for escaping
SavePath=C:\RE4_Saves
```

## How It Works

The plugin hooks into the Windows API functions `CreateFileA` and `CreateFileW` to intercept file operations. When it detects that the game is trying to save to its default location, it redirects the operation to your custom save directory.

## Building from Source

### Prerequisites

- MinGW-w64 (32-bit version)
- Microsoft Detours library
- Git

### Steps

1. Clone this repository
2. Download and build Detours library (or get pre-built version)
3. Run: `g++ -shared -O2 -Wall -Wextra -D_WIN32_WINNT=0x0501 dllmain.cpp -o RE4SaveRedirect.dll -L. -ldetours -lshlwapi`

Or use the provided batch file: `mingw_only_build.bat`

## Files Included

- `dllmain.cpp` - Main plugin source code
- `re4_save_redirect.ini` - Configuration file
- `Makefile` - For building with MinGW
- `mingw_only_build.bat` - MinGW-only build script
- `download_dependencies.bat` - Download dependencies only

## Notes

- Make sure the custom save directory exists and is writable
- Back up your save files regularly
- The plugin only affects save file operations, not other game files
- For 32-bit games like RE4 (2005), use 32-bit (x86) build configuration