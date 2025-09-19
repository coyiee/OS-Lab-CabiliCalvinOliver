#include <windows.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    DWORD pid = GetCurrentProcessId();
    printf("Child process started. PID: %lu\n", pid);
    if (argc > 1) printf("Parent PID received: %s\n", argv[1]);
    Sleep(30000);
    printf("Child %lu exiting now\n", pid);
    return 0;}