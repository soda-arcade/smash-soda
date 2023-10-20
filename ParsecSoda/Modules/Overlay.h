#pragma once

#include <cstdlib>
#include <iostream>
#include <string>
#include <tchar.h>

#include <iostream>
#include <WinSock2.h>
#include <Windows.h>
#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif
#define ASIO_STANDALONE
#define _WEBSOCKETPP_CPP11_RANDOM_DEVICE_
#define _WEBSOCKETPP_CPP11_TYPE_TRAITS_
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include <nlohmann/json.hpp>
#include "../ChatBot.h"
#include "../GamepadClient.h"

#include "ProcessMan.h"
#include "../ChatLog.h"

typedef websocketpp::client<websocketpp::config::asio_client> client;
typedef websocketpp::config::asio_client::message_type::ptr message_ptr;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;
using json = nlohmann::json;

class Overlay {
public:
	Overlay();
	void init(ProcessMan& processMan, ChatLog& chatLog);

	void start();
	void run();
	void stop();

	void sendChatMessage(Guest guest, string message);
	void sendLogMessage(string message);
	void sendGuests();
	void sendGamepads(vector<AGamepad*> gamepads);
	void sendSectionAdd(string barId, string sectionName);
	void sendSectionRemove(string barId, string sectionName);
	void sendValueAdd(string sectionId, string valueId, string valueLabel, string value);
	void sendValueModify(string valueId, string value);
	void sendValueRemove(string valueId, string sectionId);
	void sendCustomCommand(string command);

	bool isActive = false;
	bool isOpen = false;

private:
	ProcessMan* _processMan;
	ChatLog* _chatLog;

	thread _startOverlayThread;

	DWORD _pid;

	websocketpp::client<websocketpp::config::asio_client> _client;
	websocketpp::connection_hdl _handle;

	void _launchOverlay();
	void _openSocket();

	void _onOpen(client* c, websocketpp::connection_hdl hdl);
	void _onFail(client* c, websocketpp::connection_hdl hdl);
	void _sendMessage(string message);
	void _onMessage(client* c, websocketpp::connection_hdl hdl, message_ptr msg);
	void _onClose(client* c, websocketpp::connection_hdl hdl);
	
	void _log(string message);
};