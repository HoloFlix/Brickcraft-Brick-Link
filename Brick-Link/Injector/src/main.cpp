#include <windows.h>
#include <iostream>
#include <string>
#include <tlhelp32.h>

// Function to find the Process ID (PID) of a running process
DWORD GetProcessIdByName(const std::string& processName) {
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (Process32First(snapshot, &entry) == TRUE) {
        do {
            if (_stricmp(entry.szExeFile, processName.c_str()) == 0) {
                CloseHandle(snapshot);
                return entry.th32ProcessID;
            }
        } while (Process32Next(snapshot, &entry) == TRUE);
    }
    CloseHandle(snapshot);
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " \"<process_name.exe>\"" << std::endl;
        std::cerr << "Example: " << argv[0] << " \"Rex-Kwon-Do Server.exe\"" << std::endl;
        return 1;
    }
    
    // Instead of taking just argv[1], we use the argument as is.
    // The command prompt will handle the quotes.
    std::string processName = argv[1];
    
    const char* dllPath = "Patcher.dll"; // Assumes Patcher.dll is in the same folder

    std::cout << "Searching for process: " << processName << "..." << std::endl;
    DWORD procID = GetProcessIdByName(processName);

    if (procID == 0) {
        std::cerr << "Error: Process not found. Is it running?" << std::endl;
        return 1;
    }
    std::cout << "Process found! PID: " << procID << std::endl;

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procID);
    if (hProcess == NULL) {
        std::cerr << "Error: Failed to open process. Try running as Administrator." << std::endl;
        return 1;
    }
    std::cout << "Successfully opened handle to process." << std::endl;
    
    char fullDllPath[MAX_PATH];
    GetFullPathNameA(dllPath, MAX_PATH, fullDllPath, NULL);

    void* loc = VirtualAllocEx(hProcess, 0, sizeof(fullDllPath), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (loc == NULL) {
        std::cerr << "Error: Failed to allocate memory in target process." << std::endl;
        CloseHandle(hProcess);
        return 1;
    }

    if (WriteProcessMemory(hProcess, loc, fullDllPath, sizeof(fullDllPath), 0) == 0) {
        std::cerr << "Error: Failed to write to process memory." << std::endl;
        CloseHandle(hProcess);
        return 1;
    }

    HANDLE hThread = CreateRemoteThread(hProcess, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, loc, 0, 0);
    if (hThread == NULL) {
        std::cerr << "Error: Failed to create remote thread." << std::endl;
        CloseHandle(hProcess);
        return 1;
    }

    std::cout << "Success! Remote thread created. Injection sent." << std::endl;
    CloseHandle(hProcess);
    
    return 0;
}