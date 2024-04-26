#pragma once

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
#include "../Core/Config.h"
#include "../ChatBot.h"
#include "../GamepadClient.h"

#include "../ChatLog.h"
#include "../GuestList.h"
#include "../ImGui/imgui.h"

typedef websocketpp::client<websocketpp::config::asio_client> client;
typedef websocketpp::config::asio_client::message_type::ptr message_ptr;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;
using json = nlohmann::json;

class Overlay {

public:

	Overlay();
	void init(Guest& _host);

	void start();
	void stop();

	bool createConnection();
	void closeConnection();

	void sendChatMessage(Guest guest, string message);
	void sendLogMessage(string message);

	void addGuest(string userID, string name);
	void removeGuest(string userID);
	void guestPoll();

	void padAssign(uint32_t userID, string userName, int padID);
	void padUnassign(uint32_t userID);
	void padPoll();

	void updateSettings();

private:
	bool isRunning = false;
	bool isConnected = false;
	DWORD processId = 0;
	thread processThread;
	int connectionAttempts = 0;

	int messageID = 0;

	websocketpp::client<websocketpp::config::asio_client> _client;
	websocketpp::connection_hdl _handle;

	thread _createConnectionThread;

	string _json = "";

	// Stick to float
	ImVec2 stickShortToFloat(SHORT lx, SHORT ly, float& distance);

	bool _isOverlayRunning();

	void _onOpen(client* c, websocketpp::connection_hdl hdl);
	void _onFail(client* c, websocketpp::connection_hdl hdl);
	void _sendMessage(string message);
	void _onMessage(client* c, websocketpp::connection_hdl hdl, message_ptr msg);
	void _onClose(client* c, websocketpp::connection_hdl hdl);
	void _log(string message);

};