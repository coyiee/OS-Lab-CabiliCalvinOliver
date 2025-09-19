#include <windows.h>
#include <stdio.h>

int main() {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    DWORD parentPID = GetCurrentProcessId();
    printf("Parent PID: %lu\n", parentPID);

    char cmdLine[] = "child_echo.exe 1234";

    if (!CreateProcess(
            NULL, cmdLine, NULL, NULL, FALSE,
            0, NULL, NULL, &si, &pi)) {
        printf("CreateProcess failed (%d).\n", GetLastError());
        return 1;}

    WaitForSingleObject(pi.hProcess, INFINITE);

    DWORD exitCode;
    GetExitCodeProcess(pi.hProcess, &exitCode);
    printf("Child exited with code: %lu\n", exitCode);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;}