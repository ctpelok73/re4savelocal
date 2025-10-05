#pragma once

// Stub definitions for Detours library
// This is used for compilation in CI/CD environments
// Actual implementation requires the real Detours library

typedef void* PVOID;
typedef PVOID* PPVOID;

// Detour transaction functions
void DetourTransactionBegin();
void DetourTransactionCommit();
void DetourUpdateThread(void* hThread);

// Detour attachment functions
void DetourAttach(PPVOID ppPointer, PVOID pDetour);
void DetourDetach(PPVOID ppPointer, PVOID pDetour);

// For compilation purposes only
#define DetourAttach(x, y) 0
#define DetourDetach(x, y) 0
#define DetourTransactionBegin() 0
#define DetourTransactionCommit() 0
#define DetourUpdateThread(x) 0