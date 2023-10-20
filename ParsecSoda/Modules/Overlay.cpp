#include "Overlay.h"
#include "../Hosting.h"
extern Hosting g_hosting;

/// <summary>
/// Constructor
/// </summary>
Overlay::Overlay() : _processMan(nullptr), _chatLog(nullptr) {}

/// <summary>
/// Initialization
/// </summary>
/// <param name="gamepadClient"></param>
void Overlay::init(ProcessMan& processMan, ChatLog& chatLog) {
	_processMan = &processMan;
	_chatLog = &chatLog;

	// set logging policy if needed
	_client.clear_access_channels(websocketpp::log::alevel::frame_header);
	_client.clear_access_channels(websocketpp::log::alevel::frame_payload);

	// Initialize ASIO
	_client.init_asio();

	// Register our handlers
	_client.set_open_handler(bind(&Overlay::_onOpen, this, &_client, ::_1));
	_client.set_fail_handler(bind(&Overlay::_onFail, this, &_client, ::_1));
	_client.set_message_handler(bind(&Overlay::_onMessage, this, &_client, ::_1, ::_2));
	_client.set_close_handler(bind(&Overlay::_onClose, this, &_client, ::_1));
	
}

/// <summary>
/// Launch the overlay
/// </summary>
void Overlay::start() {

	if (isActive) {
		stop();
	} else {

		// Start setting up the overlay
		_startOverlayThread = thread([&]() {

			_launchOverlay();
			Sleep(10000);

			if (!_processMan->isProcessRunning(_pid)) {
				_log("The overlay application could not be started.");
				return;
			}

			_log("Attempting to connect to overlay...");

			// Create a connection to the given URI and queue it for connection once
			// the event loop starts
			websocketpp::lib::error_code ec;
			client::connection_ptr con = _client.get_connection("ws://localhost:9002", ec);
			_client.connect(con);
			_client.run();

		});

		// Finish
		_startOverlayThread.detach();

	}

}

/// <summary>
/// Handles the overlay loop
/// </summary>
void Overlay::run() {

	// Is overlay active?
	if (isActive) {
		stop();
	}

}

/// <summary>
/// Stops the overlay.
/// </summary>
void Overlay::stop() {
	
	// Close the socket
	_client.close(_handle, websocketpp::close::status::normal, "Closing connection");

	// Stop loop
	isActive = false;

}

/// <summary>
/// Launches the overlay application.
/// </summary>
void Overlay::_launchOverlay() {

	_log("Attempting to launch overlay...");

	// Get relative path
	wchar_t buffer[MAX_PATH];
	GetModuleFileNameW(NULL, buffer, MAX_PATH);
	std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
	std::wstring path = std::wstring(buffer).substr(0, pos);
	path += L"\\overlay\\Smash-Soda-Overlay.exe";
	std::string pathStr = std::string(path.begin(), path.end());

	_pid = _processMan->start(pathStr, "");

}

/// <summary>
/// Opens a web socket connection with the overlay.
/// </summary>
void Overlay::_openSocket() {

	// Server address
	std::string uri = "ws://localhost:9002";

	try {

		// set logging policy if needed
		_client.clear_access_channels(websocketpp::log::alevel::frame_header);
		_client.clear_access_channels(websocketpp::log::alevel::frame_payload);
		//c.set_error_channels(websocketpp::log::elevel::none);

		// Initialize ASIO
		_client.init_asio();

		// Register our handlers
		_client.set_open_handler(bind(&Overlay::_onOpen, this, &_client, ::_1));
		_client.set_fail_handler(bind(&Overlay::_onFail, this, &_client, ::_1));
		_client.set_message_handler(bind(&Overlay::_onMessage, this, &_client, ::_1, ::_2));
		_client.set_close_handler(bind(&Overlay::_onClose, this, &_client, ::_1));

		// Create a connection to the given URI and queue it for connection once
		// the event loop starts
		websocketpp::lib::error_code ec;
		client::connection_ptr con = _client.get_connection(uri, ec);
		_client.connect(con);

		// Start the ASIO io_service run loop
		_client.run();
		isActive = true;

		_log("The overlay has been started. Ctrl+Shift+C toggles host chat box for chatting with your guests.");
		
	}
	catch (const std::exception& e) {
		/*ostringstream out;
		out << e.what();
		_log(out.str());*/
		_log("The overlay application could not be started.");
	}
	catch (websocketpp::lib::error_code e) {
		/*ostringstream out;
		out << e.message();
		_log(out.str());*/
		_log("The overlay application could not be started.");
	}
	catch (...) {
		/*ostringstream out;
		out << "other exception";
		_log(out.str());*/
		_log("The overlay application could not be started.");
	}

}

/// <summary>
/// Socket opened successfully event.
/// </summary>
/// <param name="c"></param>
/// <param name="hdl"></param>
void Overlay::_onOpen(client* c, websocketpp::connection_hdl hdl) {
	isActive = true;
	_handle = hdl;
	_log("The overlay has been started. Ctrl+Shift+C toggles host chat box for chatting with your guests.");
}

/// <summary>
/// Connected failed event.
/// </summary>
/// <param name="c"></param>
/// <param name="hdl"></param>
void Overlay::_onFail(client* c, websocketpp::connection_hdl hdl) {
	MetadataCache::preferences.overlayShow = false;
	_log("The overlay application could not be started.");
}

/// <summary>
/// Message recieved event.
/// </summary>
/// <param name="c"></param>
/// <param name="hdl"></param>
/// <param name="msg"></param>
void Overlay::_onMessage(client* c, websocketpp::connection_hdl hdl, message_ptr msg) {

	//_chatBot->setLastUserId(-1);
	//_chatLog->logCommand("[WebSocket] " + msg->get_payload());

	//_chatBot->setLastUserId(_host->userID);

	//CommandDefaultMessage defaultMessage(message, guest, _chatBot->getLastUserId(), tier);

	json j = json::parse(msg->get_payload());

	if (j["type"] == "message") {

		string message;
		j.at("message").get_to(message);
		ostringstream test;
		test << message;
		g_hosting.sendHostMessage(test.str().c_str());

	}

}

/// <summary>
/// Connection closed event.
/// </summary>
/// <param name="c"></param>
/// <param name="hdl"></param>
void Overlay::_onClose(client* c, websocketpp::connection_hdl hdl) {
	_log("The overlay application has been closed.");
}

/// <summary>
/// Sends a chat message to the web socket.
/// </summary>
/// <param name="guest"></param>
/// <param name="message"></param>
void Overlay::sendChatMessage(Guest guest, string message) {

	if (!isActive) {
		return;
	}

	json j;
	j["id"] = -1;
	j["type"] = "chat";
	j["guest"] = {
		{"id", guest.id},
		{"userID", guest.userID},
		{"name", guest.name},
	};
	j["message"] = {
		{"type", "chat"},
		{"message", message}
	};

	// Send message through web socket
	_sendMessage(j.dump());

}

/// <summary>
/// Sends a log message to the web socket.
/// </summary>
/// <param name="message"></param>
void Overlay::sendLogMessage(string message) {

	json j;
	j["id"] = -1;
	j["type"] = "chat";
	j["message"] = {
		{"type", "log"},
		{"message", message}
	};

	// Send message through web socket
	_sendMessage(j.dump());

}

/// <summary>
/// Sends a custom message type to the web socket.
/// </summary>
/// <param name="command"></param>
void Overlay::sendCustomCommand(string command) {

}

void Overlay::sendGuests() {

}

void Overlay::sendGamepads(vector<AGamepad*> gamepads) {

	json j;
	j["id"] = -1;
	j["type"] = "gamepads";

	// Add gamepads
	json arr = json::array();

	// Iterate through gamepads
	for (int i = 0; i < gamepads.size(); i++) {

		// Json object
		json obj;

		// Check if gamepad is being held by guest
		bool isOwned = gamepads[i]->isOwned();

		// Add gamepad properties
		obj["index"] = gamepads[i]->getIndex();
		obj["owner"] = (isOwned ? gamepads[i]->owner.guest.name : "");
		obj["isOwned"] = isOwned;
		obj["isLocked"] = gamepads[i]->isLocked();
		obj["isConnected"] = gamepads[i]->isConnected();

		// Add gamepad to array
		arr.push_back(obj);

	}

	// Add gamepads to json
	j["gamepads"] = arr;

	// Send message through web socket
	_sendMessage(j.dump());

}

/// <summary>
/// Sends a message to the web socket.
/// </summary>
/// <param name="message"></param>
void Overlay::_sendMessage(string message) {

	// Send message through web socket
	_client.send(_handle, message, websocketpp::frame::opcode::text);

}

/// <summary>
/// Sends a new section to add to a bar.
/// </summary>
/// <param name="barId"></param>
/// <param name="sectionName"></param>
void Overlay::sendSectionAdd(string barId, string sectionName) {

}

/// <summary>
/// Removes a section from a bar.
/// </summary>
/// <param name="barId"></param>
/// <param name="sectionName"></param>
void Overlay::sendSectionRemove(string barId, string sectionName) {

}

/// <summary>
/// Sends a new value to add to a section.
/// </summary>
/// <param name="sectionId"></param>
/// <param name="valueId"></param>
/// <param name="valueLabel"></param>
/// <param name="value"></param>
void Overlay::sendValueAdd(string sectionId, string valueId, string valueLabel, string value) {

}

/// <summary>
/// Sends a value to modify.
/// </summary>
/// <param name="valueId"></param>
/// <param name="value"></param>
void Overlay::sendValueModify(string valueId, string value) {

}

/// <summary>
/// Sends a value to remove from a bar.
/// </summary>
/// <param name="valueId"></param>
/// <param name="sectionId"></param>
void Overlay::sendValueRemove(string valueId, string sectionId) {

}

/// <summary>
/// Helper function for logging a message to the chat log.
/// </summary>
void Overlay::_log(string message) {
	_chatLog->logCommand("[Overlay] " + message);
}