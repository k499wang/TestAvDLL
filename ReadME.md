# DLL Hooking Example with Detours

This project demonstrates how to hook the `MessageBoxW` and `MessageBoxA` functions in Windows using the [Microsoft Detours](https://github.com/microsoft/Detours) library. The hooks allow for custom behavior while preserving the original functionality of these API calls.

## Features

- Hooks the `MessageBoxW` and `MessageBoxA` functions to intercept and log their parameters.
- Displays a message box when the DLL is injected into a process.
- Safely detaches hooks when the DLL is unloaded.

## Requirements

- **Windows Development Environment**
- **Visual Studio**: To compile the project.
- **Microsoft Detours Library**: Ensure the library is downloaded and integrated into your project.

## How It Works

1. **Hooked Functions**:
   - `HookedMessageBoxW` and `HookedMessageBoxA` log the arguments passed to `MessageBoxW` and `MessageBoxA` respectively.
   - The original functions are called after logging to maintain their behavior.

2. **Hook Installation**:
   - The `Hooker()` function sets up the hooks using Detours.
   - The hooks are applied when the DLL is injected into the target process (`DLL_PROCESS_ATTACH`).

3. **Hook Removal**:
   - The `UnHooker()` function removes the hooks when the DLL is unloaded (`DLL_PROCESS_DETACH`).

4. **DLL Lifecycle**:
   - The `DllMain` function manages the attachment and detachment of hooks based on the DLL's loading and unloading events.

## Usage

In https://github.com/k499wang/CustomAV, use the testFile.py and the compiled directory in the same folder:

```
testFile.py -f <filename>
```
In order to inject the dll in the process and run it.

