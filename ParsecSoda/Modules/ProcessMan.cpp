#include "ProcessMan.h"

/// <summary>
/// Constructor
/// </summary>
ProcessMan::ProcessMan() : _chatLog(nullptr) {}

/// <summary>
/// Initialization
/// </summary>
void ProcessMan::init(ChatLog& chatLog) {
	_chatLog = &chatLog;
}

/// <summary>
/// Starts watching for a process
/// </summary>
/// <param name="path"></param>
/// <param name="args"></param>
/// <returns></returns>
DWORD ProcessMan::start(std::string path, std::string args) {

    // Convert path and args to LPCWSTR
    std::wstring wpath(path.begin(), path.end());
    std::wstring wargs(args.begin(), args.end());
    LPCWSTR path_cstr = wpath.c_str();
    LPCWSTR args_cstr = wargs.c_str();

    // Start an application
    pid = launch(path_cstr, args_cstr);

	// Start thread to always check if the process is running
    if (pid != 0) {
		_isRunning = true;
		_thread = thread([this, path_cstr, args_cstr]() {
            while (_isRunning) {
                if (!isProcessRunning(pid)) {
                    // Relaunch
					pid = launch(path_cstr, args_cstr);
                }
                this_thread::sleep_for(chrono::milliseconds(3000));
            }
        });
		_thread.detach();
    }
    
    return pid;
    
}

/// <summary>
/// Stops watching for a process
/// </summary>
void ProcessMan::stop() {
    // Signal the monitoring thread to stop
    _isRunning = false;

    // Reset pid
    pid = 0;
}

/// <summary>
/// Launches a process
/// </summary>
/// <param name="path_cstr"></param>
/// <param name="args_cstr"></param>
/// <returns></returns>
DWORD ProcessMan::launch(LPCWSTR path_cstr, LPCWSTR args_cstr) {

    // Start an application
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);

    // Start the process and return the process ID
    if (CreateProcess(path_cstr, (LPWSTR)args_cstr, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        DWORD pid = pi.dwProcessId;

        // Close process and thread handles
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        return pid;
    }
    else {
        return 0; // Return 0 if the process could not be created
    }
}

/// <summary>
/// Kills a process
/// </summary>
/// <param name="pid"></param>
/// <returns></returns>
bool ProcessMan::kill(DWORD pid) {

    // Kill a process
    HANDLE process = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
    if (process != NULL) {
        BOOL result = TerminateProcess(process, 0);
        CloseHandle(process);
        return result != 0;
    }
    else {
        return false;
    }
}

/// <summary>
/// Checks if a process is running
/// </summary>
/// <param name="pid"></param>
/// <returns></returns>
bool ProcessMan::isProcessRunning(DWORD pid) {
    HANDLE process = OpenProcess(SYNCHRONIZE, FALSE, pid);
    DWORD exitCode;

    if (process == NULL) {
        // Failed to open the process, assume it's not running
        return false;
    }

    if (!GetExitCodeProcess(process, &exitCode)) {
        // Failed to get the exit code, assume the process is running
        CloseHandle(process);
        return true;
    }

    CloseHandle(process);
    return exitCode == STILL_ACTIVE;
}