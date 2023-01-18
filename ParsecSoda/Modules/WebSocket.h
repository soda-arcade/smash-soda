#pragma once

#include "../ChatLog.h"

class WebSocket {

public:
	WebSocket();
	void init(ChatLog& chatLog);

private:
	ChatLog* _chatLog;

	// Large buffer we can reuse
	std::vector<char> _vBuffer = std::vector<char>(20 * 1024);
	std::vector<char> _response = std::vector<char>(20 * 1024);

	bool createSocket();
	void getData();

};