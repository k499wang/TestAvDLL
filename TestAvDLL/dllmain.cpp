#include "pch.h"
#include <wchar.h>
#include <stdio.h>
#include <windows.h>
#include <detours/detours.h>

// Pointers to the original MessageBox functions
int (WINAPI* pOrigMessageBoxW)(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType) = MessageBoxW;
int (WINAPI* pOrigMessageBoxA)(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType) = MessageBoxA;

// Hooked functions
int HookedMessageBoxW(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType) {
    // Print out the arguments
    wprintf(L"HookedMessageBoxW() called:\n");
    wprintf(L"  lpText: %s\n", lpText);
    wprintf(L"  lpCaption: %s\n", lpCaption);
    wprintf(L"  uType: %u\n", uType);

    // Call the original MessageBoxW function
    return pOrigMessageBoxW(hWnd, lpText, lpCaption, uType);
}

int HookedMessageBoxA(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType) {
    // Print out the arguments
    printf("HookedMessageBoxA() called:\n");
    printf("  lpText: %s\n", lpText);
    printf("  lpCaption: %s\n", lpCaption);
    printf("  uType: %u\n", uType);

    // Call the original MessageBoxA function
    return pOrigMessageBoxA(hWnd, lpText, lpCaption, uType);
}

// Set hooks on MessageBoxW and MessageBoxA
BOOL Hooker(void) {
    LONG err;

    DetourTransactionBegin(); // Start a hooking transaction
    DetourUpdateThread(GetCurrentThread()); // Ensure hook is applied to the current thread
    DetourAttach(&(PVOID&)pOrigMessageBoxW, HookedMessageBoxW); // Attach hook to MessageBoxW, this accepts a pointer to the function pointer you want to change
    DetourAttach(&(PVOID&)pOrigMessageBoxA, HookedMessageBoxA); // Attach hook to MessageBoxA
    err = DetourTransactionCommit(); // Commit the transaction to apply the hooks

    if (err != NO_ERROR) {
        printf("Error in DetourTransactionCommit during hook: %ld\n", err);
        return FALSE;
    }

    printf("MessageBoxW() and MessageBoxA() hooked!\n");

    return TRUE;
}

// Revert all changes to the original code
BOOL UnHooker(void) {
    LONG err;

    DetourTransactionBegin(); // Start a detachment transaction
    DetourUpdateThread(GetCurrentThread()); // Update the current thread
    DetourDetach(&(PVOID&)pOrigMessageBoxW, HookedMessageBoxW); // Detach hook from MessageBoxW
    DetourDetach(&(PVOID&)pOrigMessageBoxA, HookedMessageBoxA); // Detach hook from MessageBoxA
    err = DetourTransactionCommit(); // Commit the transaction to remove the hooks

    if (err != NO_ERROR) {
        printf("Error in DetourTransactionCommit during unhook: %ld\n", err);
        return FALSE;
    }

    printf("Hooks removed from MessageBoxW() and MessageBoxA()!\n");

    return TRUE;
}

BOOL WINAPI DllMain(HINSTANCE hinst, DWORD dwReason, LPVOID reserved) {
    switch (dwReason) {
    case DLL_PROCESS_ATTACH:
		MessageBoxA(NULL, "DLL Injected!", "DLL", MB_OK); // Display a message box when the DLL is injected
        Hooker();
        break;

    case DLL_PROCESS_DETACH:
        UnHooker(); // Unhook the MessageBox functions when the DLL is unloaded
        break;

    default:
        break;
    }

    return TRUE;
}
