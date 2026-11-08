
/* winprocess.c — Process creation on Windows using CreateProcess() */

#include <stdio.h>
#include <windows.h>

int main() {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    // Initialize structures to zero
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Display parent process PID
    printf("Parent process (PID: %lu) — creating child...\n",
           GetCurrentProcessId());

    // Create the child process (Microsoft Paint)
    if (!CreateProcess(
            "C:\\Windows\\System32\\mspaint.exe",
            NULL,       // Command line
            NULL,       // Process security attributes
            NULL,       // Thread security attributes
            FALSE,      // Inherit handles
            0,          // Creation flags
            NULL,       // Environment
            NULL,       // Current directory
            &si,        // Startup information
            &pi         // Process information
        ))
    {
        fprintf(stderr, "CreateProcess failed (error %lu).\n",
                GetLastError());
        return 1;
    }

    // Display child process information
    printf("Child process created!\n");
    printf("  Child PID:       %lu\n", pi.dwProcessId);
    printf("  Child Thread ID: %lu\n", pi.dwThreadId);

    printf("Parent: waiting for child (mspaint) to exit...\n");
    printf(">>> Open Task Manager and observe the processes! <<<\n");
    printf(">>> Close mspaint when you finish observing. <<<\n");

    // Wait until the child process terminates
    DWORD result = WaitForSingleObject(pi.hProcess, INFINITE);

    if (result == WAIT_FAILED) {
        fprintf(stderr, "WaitForSingleObject failed (error %lu).\n",
                GetLastError());
    }
    else {
        printf("Parent: child has exited.\n");
    }

    // Release process and thread handles
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    printf("Parent: done. Cleaning up.\n");

    return 0;
}
