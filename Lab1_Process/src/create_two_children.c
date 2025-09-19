#include <windows.h>
#include <stdio.h>

int main() {
    STARTUPINFO si[2];
    PROCESS_INFORMATION pi[2];
    DWORD parentPid = GetCurrentProcessId();
    printf("Parent PID: %lu\n", parentPid);

    for (int i = 0; i < 2; i++) {
        ZeroMemory(&si[i], sizeof(si[i]));
        si[i].cb = sizeof(si[i]);
        ZeroMemory(&pi[i], sizeof(pi[i]));

        char cmd[100];
        sprintf(cmd, "child_echo.exe %lu", parentPid);

        if (!CreateProcess(NULL, cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si[i], &pi[i])) {
            printf("CreateProcess failed (%lu).\n", GetLastError());
            return 1; } }

    HANDLE handles[2] = {pi[0].hProcess, pi[1].hProcess};
    WaitForMultipleObjects(2, handles, TRUE, INFINITE);

    for (int i = 0; i < 2; i++) {
        DWORD exitCode;
        GetExitCodeProcess(pi[i].hProcess, &exitCode);
        printf("Child %d exited with code %lu\n", i + 1, exitCode);
        CloseHandle(pi[i].hProcess);
        CloseHandle(pi[i].hThread);}

    return 0;}