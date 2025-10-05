#ifdef _WIN32
#include <windows.h>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <shlobj.h>

#ifndef _USE_DETOURS_STUB
#include <detours.h>
#pragma comment(lib, "detours.lib")
#else
// Include stub definitions when building in CI/CD without real Detours
#include "detours_stub.h"
#endif

// Original function pointers
typedef HANDLE (WINAPI *CreateFileAPtr)(
    LPCSTR lpFileName,
    DWORD dwDesiredAccess,
    DWORD dwShareMode,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD dwCreationDisposition,
    DWORD dwFlagsAndAttributes,
    HANDLE hTemplateFile
);

typedef HANDLE (WINAPI *CreateFileWPtr)(
    LPCWSTR lpFileName,
    DWORD dwDesiredAccess,
    DWORD dwShareMode,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD dwCreationDisposition,
    DWORD dwFlagsAndAttributes,
    HANDLE hTemplateFile
);

CreateFileAPtr TrueCreateFileA = CreateFileA;
CreateFileWPtr TrueCreateFileW = CreateFileW;

// Configuration variables
std::string customSavePath = "";
bool configLoaded = false;
std::vector<std::string> knownSavePatterns = {
    "Save", "slot", "gamedata", "config", ".dat", ".bin", ".sav", "game"
};

// Function to convert string to lowercase for comparison
std::string ToLower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

// Function to load configuration
void LoadConfig() {
    if (configLoaded) return;
    
    // Default to current directory + saves
    char modulePath[MAX_PATH];
    GetModuleFileNameA(NULL, modulePath, MAX_PATH);
    std::string moduleDir = std::string(modulePath);
    size_t lastSlash = moduleDir.find_last_of("\\/");
    if (lastSlash != std::string::npos) {
        moduleDir = moduleDir.substr(0, lastSlash);
    }
    
    std::string configPath = moduleDir + "\\re4_save_redirect.ini";
    
    // Read custom save path from config file
    char buffer[512];
    GetPrivateProfileStringA("Settings", "SavePath", "", buffer, sizeof(buffer), configPath.c_str());
    
    if (strlen(buffer) > 0) {
        customSavePath = std::string(buffer);
        // Ensure path ends with backslash
        if (customSavePath.back() != '\\' && customSavePath.back() != '/') {
            customSavePath += "\\";
        }
        
        // Create directory if it doesn't exist
        CreateDirectoryA(customSavePath.c_str(), NULL);
    }
    
    configLoaded = true;
}

// Check if the file is a RE4 save file
bool IsRe4SaveFile(const std::string& fileName) {
    std::string lowerFileName = ToLower(fileName);
    
    // Check for common RE4 save file patterns
    for (const auto& pattern : knownSavePatterns) {
        if (lowerFileName.find(ToLower(pattern)) != std::string::npos) {
            return true;
        }
    }
    
    // Additional checks for specific RE4 file extensions
    if (lowerFileName.find(".dat") != std::string::npos ||
        lowerFileName.find(".bin") != std::string::npos ||
        lowerFileName.find(".sav") != std::string::npos) {
        return true;
    }
    
    return false;
}

// Redirect file path if it's a RE4 save file
std::string RedirectFilePath(const std::string& originalPath) {
    LoadConfig();
    
    if (customSavePath.empty()) {
        return originalPath; // No redirection if path not configured
    }
    
    // Extract just the filename
    size_t lastSlash = originalPath.find_last_of("\\/");
    std::string fileName = (lastSlash != std::string::npos) ?
                          originalPath.substr(lastSlash + 1) : originalPath;
    
    // Check if this is a RE4 save file
    if (IsRe4SaveFile(fileName)) {
        return customSavePath + fileName;
    }
    
    return originalPath;
}

// Hooked CreateFileA function
HANDLE WINAPI HookedCreateFileA(
    LPCSTR lpFileName,
    DWORD dwDesiredAccess,
    DWORD dwShareMode,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD dwCreationDisposition,
    DWORD dwFlagsAndAttributes,
    HANDLE hTemplateFile
) {
    if (lpFileName) {
        std::string redirectedPath = RedirectFilePath(std::string(lpFileName));
        return TrueCreateFileA(redirectedPath.c_str(), dwDesiredAccess, dwShareMode,
                              lpSecurityAttributes, dwCreationDisposition,
                              dwFlagsAndAttributes, hTemplateFile);
    }
    return TrueCreateFileA(lpFileName, dwDesiredAccess, dwShareMode,
                          lpSecurityAttributes, dwCreationDisposition,
                          dwFlagsAndAttributes, hTemplateFile);
}

// Hooked CreateFileW function
HANDLE WINAPI HookedCreateFileW(
    LPCWSTR lpFileName,
    DWORD dwDesiredAccess,
    DWORD dwShareMode,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD dwCreationDisposition,
    DWORD dwFlagsAndAttributes,
    HANDLE hTemplateFile
) {
    if (lpFileName) {
        std::string originalPath(lpFileName);
        std::string redirectedPath = RedirectFilePath(originalPath);
        return TrueCreateFileW(redirectedPath.c_str(), dwDesiredAccess, dwShareMode,
                              lpSecurityAttributes, dwCreationDisposition,
                              dwFlagsAndAttributes, hTemplateFile);
    }
    return TrueCreateFileW(lpFileName, dwDesiredAccess, dwShareMode,
                          lpSecurityAttributes, dwCreationDisposition,
                          dwFlagsAndAttributes, hTemplateFile);
}

// DLL Entry Point
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        // Disable DLL_THREAD_ATTACH and DLL_THREAD_DETACH notifications for performance
        DisableThreadLibraryCalls(hModule);
        
#ifdef _USE_DETOURS_STUB
        // In CI/CD environment, don't actually hook functions
        break;
#else
        // Initialize detours
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        
        // Attach hooks
        DetourAttach(&(PVOID&)TrueCreateFileA, HookedCreateFileA);
        DetourAttach(&(PVOID&)TrueCreateFileW, HookedCreateFileW);
        
        DetourTransactionCommit();
        break;
#endif
        
    case DLL_PROCESS_DETACH:
#ifndef _USE_DETOURS_STUB
        // Remove hooks
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        
        DetourDetach(&(PVOID&)TrueCreateFileA, HookedCreateFileA);
        DetourDetach(&(PVOID&)TrueCreateFileW, HookedCreateFileW);
        
        DetourTransactionCommit();
#endif
        break;
    }
    
    return TRUE;
}