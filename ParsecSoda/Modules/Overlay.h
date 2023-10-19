#pragma once

#include <cstdlib>
#include <iostream>
#include <string>
#include <tchar.h>
#include "ProcessMan.h"
#include "WebSocket.h"
#include "../ChatLog.h"

class Overlay {
public:
	Overlay();
	void init(ProcessMan& processMan, WebSocket& webSocket, ChatLog& chatLog);

	void start();
	void run();
	void stop();

	bool isActive = false;

private:
	ProcessMan* _processMan;
	WebSocket* _webSocket;
	ChatLog* _chatLog;

	thread _startOverlayThread;

	DWORD _pid;

	bool _launchOverlay();
	bool _openSocket();
	void _log(string message);
};