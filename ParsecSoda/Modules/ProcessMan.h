#pragma once

#if !defined(_WIN32)
	#include <Windows.h>
#endif

#include <atlbase.h>
#include <atlconv.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <tchar.h>
#include <thread>
#include <atomic>
#include "../ChatLog.h"
#include "../ChatBot.h"

class ProcessMan {
public:
	ProcessMan();
	void init(ChatLog& chatLog);

	DWORD start(string path, string args);
	DWORD launch(LPCWSTR path_cstr, LPCWSTR args_cstr);
	void stop();
	bool kill(DWORD pid);
	bool isProcessRunning(DWORD pid);

private:
	std::atomic<bool> _isRunning = false;
	DWORD pid;
	ChatLog* _chatLog;
	thread _thread;
	LPWSTR path_cstr;
	LPWSTR args_cstr;
};