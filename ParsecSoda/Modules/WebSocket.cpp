#include "WebSocket.h"
#include "../Hosting.h"
extern Hosting g_hosting;

/// <summary>
/// Constructor
/// </summary>
WebSocket::WebSocket() : _guestList(nullptr), _gamepadClient(nullptr), _chatBot(nullptr), _chatLog(nullptr), _host(nullptr) {}

/// <summary>
/// Initialization
/// </summary>
/// <param name="gamepadClient"></param>
void WebSocket::init(GuestList& guestList, GamepadClient& gamepadClient, ChatBot& chatBot, ChatLog& chatLog, Guest& _host) {
    _guestList = &guestList;
    _gamepadClient = &gamepadClient;
	_chatBot = &chatBot;
	_chatLog = &chatLog;
	_host = _host;
}

/// <summary>
/// Creates a new connection.
/// </summary>
/// <returns>True if successful</returns>
bool WebSocket::createConnection() {

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
        _client.set_open_handler(bind(&WebSocket::_onOpen, this, &_client, ::_1));
        _client.set_fail_handler(bind(&WebSocket::_onFail, this, &_client, ::_1));
        _client.set_message_handler(bind(&WebSocket::_onMessage, this, &_client, ::_1, ::_2));
        _client.set_close_handler(bind(&WebSocket::_onClose, this, &_client, ::_1));

        // Create a connection to the given URI and queue it for connection once
        // the event loop starts
        websocketpp::lib::error_code ec;
        client::connection_ptr con = _client.get_connection(uri, ec);
        _client.connect(con);

        // Store the connection handle
        _handle = con->get_handle();

        // Start the ASIO io_service run loop
        _client.run();
    }
    catch (const std::exception& e) {
        ostringstream out;
        out << e.what();
        _log(out.str());
        return false;
    }
    catch (websocketpp::lib::error_code e) {
        ostringstream out;
        out << e.message();
        _log(out.str());
        return false;
    }
    catch (...) {
        ostringstream out;
        out << "other exception";
        _log(out.str());
        return false;
    }

	return true;
}

void WebSocket::closeConnection() {
    _client.close(_handle, websocketpp::close::status::normal, "Closing connection");
}

/// <summary>
/// Sends a chat message to the web socket.
/// </summary>
/// <param name="guest"></param>
/// <param name="message"></param>
void WebSocket::sendChatMessage(Guest guest, string message) {

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
void WebSocket::sendLogMessage(string message) {

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

void WebSocket::sendGuests() {
    
}

void WebSocket::sendGamepads(vector<AGamepad*> gamepads) {
    
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
/// Sends a new section to add to a bar.
/// </summary>
/// <param name="barId"></param>
/// <param name="sectionName"></param>
void WebSocket::sendSectionAdd(string barId, string sectionName) {
    
}

/// <summary>
/// Removes a section from a bar.
/// </summary>
/// <param name="barId"></param>
/// <param name="sectionName"></param>
void WebSocket::sendSectionRemove(string barId, string sectionName) {
    
}

/// <summary>
/// Sends a new value to add to a section.
/// </summary>
/// <param name="sectionId"></param>
/// <param name="valueId"></param>
/// <param name="valueLabel"></param>
/// <param name="value"></param>
void WebSocket::sendValueAdd(string sectionId, string valueId, string valueLabel, string value) {
    
}

/// <summary>
/// Sends a value to modify.
/// </summary>
/// <param name="valueId"></param>
/// <param name="value"></param>
void WebSocket::sendValueModify(string valueId, string value) {
    
}

/// <summary>
/// Sends a value to remove from a bar.
/// </summary>
/// <param name="valueId"></param>
/// <param name="sectionId"></param>
void WebSocket::sendValueRemove(string valueId, string sectionId) {
    
}

/// <summary>
/// Sends a custom message type to the web socket.
/// </summary>
/// <param name="command"></param>
void WebSocket::sendCustomCommand(string command) {
    
}

/// <summary>
/// Sends a message to the web socket.
/// </summary>
/// <param name="message"></param>
void WebSocket::_sendMessage(string message) {

	if (isOpen) {

		// Send message through web socket
		_client.send(_handle, message, websocketpp::frame::opcode::text);

	}

}

/// <summary>
/// Socket opened successfully event.
/// </summary>
/// <param name="c"></param>
/// <param name="hdl"></param>
void WebSocket::_onOpen(client* c, websocketpp::connection_hdl hdl) {
    _handle = hdl;
    isOpen = true;
}

/// <summary>
/// Connected failed event.
/// </summary>
/// <param name="c"></param>
/// <param name="hdl"></param>
void WebSocket::_onFail(client* c, websocketpp::connection_hdl hdl) {
    c->get_alog().write(websocketpp::log::alevel::app, "Connection Failed");
}

/// <summary>
/// Message recieved event.
/// </summary>
/// <param name="c"></param>
/// <param name="hdl"></param>
/// <param name="msg"></param>
void WebSocket::_onMessage(client* c, websocketpp::connection_hdl hdl, message_ptr msg) {
    
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
void WebSocket::_onClose(client* c, websocketpp::connection_hdl hdl) {
    c->get_alog().write(websocketpp::log::alevel::app, "Connection Closed");
    isOpen = false;
}

/// <summary>
/// Prints message in chat log.
/// </summary>
/// <param name="message"></param>
void WebSocket::_log(string message) {

    _chatLog->logCommand("[WebSocket] " + message);

}