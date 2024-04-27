#include "Overlay.h"
#include "../Hosting.h"
extern Hosting g_hosting;

/// <summary>
/// Constructor
/// </summary>
Overlay::Overlay() {}

/// <summary>
/// Initialization
/// </summary>
/// <param name="gamepadClient"></param>
void Overlay::init(Guest& _host) {
    _host = _host;
}

/// <summary>
/// Launches an application.
/// </summary>
/// <param name="path"></param>
/// <param name="args"></param>
void Overlay::start() {

    // If already running, stop the process
    if (isRunning) {
        return;
    }

    messageID = 0;

    // Lambda thread to start the process
    _createConnectionThread = thread([this]() {

        // Wait for the process to start fully
        while (!isConnected) {
            connectionAttempts++;
            if (connectionAttempts > 10) {
                g_hosting.logMessage("Failed to launch the overlay. See this guide for instructions: https://github.com/mickeyuk/smash-soda/wiki/overlay");
                isRunning = false;
                _isOverlayRunning();
                return;
            }
            Sleep(5000);
            createConnection();
        }

    //    // Get the path of the executable
    //    wchar_t exePath[MAX_PATH];
    //    GetModuleFileName(NULL, exePath, MAX_PATH);

    //    // Extract the directory from the path
    //    wchar_t* lastBackslash = wcsrchr(exePath, L'\\');
    //    if (lastBackslash != NULL) {
    //        *lastBackslash = L'\0'; // Replace the last backslash with null terminator
    //    }

    //    // Construct the full path to the overlay executable
    //    wchar_t overlayPath[MAX_PATH];
    //    swprintf_s(overlayPath, MAX_PATH, L"%s\\overlay\\SmashSodaOverlay.exe", exePath);

    //    // Create the process
    //    STARTUPINFO si;
    //    PROCESS_INFORMATION pi;

    //    ZeroMemory(&si, sizeof(si));
    //    si.cb = sizeof(si);
    //    ZeroMemory(&pi, sizeof(pi));

    //    // Start the child process. 
    //    if (!CreateProcess(overlayPath,	// No module name (use command line)
    //        (LPWSTR)"",			        // Command line
    //        NULL,						// Process handle not inheritable
    //        NULL,						// Thread handle not inheritable
    //        FALSE,						// Set handle inheritance to FALSE
    //        0,							// No creation flags
    //        NULL,						// Use parent's environment block
    //        NULL,						// Use parent's starting directory 
    //        &si,						// Pointer to STARTUPINFO structure
    //        &pi)						// Pointer to PROCESS_INFORMATION structure
    //        ) {

    //        g_hosting.logMessage("Failed to launch the overlay. See this guide for instructions: https://github.com/mickeyuk/smash-soda/wiki/overlay");
    //        isRunning = false;

    //        return;
    //    }
    //    else {
    //        g_hosting.logMessage("Connecting to overlay...");

    //        // Wait for the process to start fully
    //        while (!isConnected) {
    //            connectionAttempts++;
    //            if (connectionAttempts > 3) {
				//	g_hosting.logMessage("Failed to connect to the overlay. Please make sure the overlay is running.");
				//	isRunning = false;
				//	return;
				//}
    //            Sleep(10000);
    //            createConnection();
    //        }
    //    }

    //    // Save the process ID
    //    processId = pi.dwProcessId;

    });

    // Start the thread
    _createConnectionThread.detach();

}

/// <summary>
/// Bye bye
/// </summary>
void Overlay::stop() {

    if (isConnected) {
        isConnected = false;
		closeConnection();
	}

	isRunning = false;

}

/// <summary>
/// Creates a new connection.
/// </summary>
/// <returns>True if successful</returns>
bool Overlay::createConnection() {

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

    _log("Connected to the overlay!");

    return true;
}

/// <summary>
/// Closes the connection to the overlay.
/// </summary>
void Overlay::closeConnection() {
    _client.close(_handle, websocketpp::close::status::normal, "Closing connection");
}

/// <summary>
/// Sends a chat message to the web socket.
/// </summary>
/// <param name="guest"></param>
/// <param name="message"></param>
void Overlay::sendChatMessage(Guest guest, string message) {

    if (!isConnected) {
        return;
    }

    json j;

    j["event"] = "chat:message";
    j["data"]["id"] = messageID;
    j["data"]["user"] = {
        {"id", guest.userID},
        {"name", guest.name},
    };
    j["data"]["message"] = message;

    // Send message through web socket
    _sendMessage(j.dump());

    messageID++;

}

/// <summary>
/// Sends a log message to the web socket.
/// </summary>
/// <param name="message"></param>
void Overlay::sendLogMessage(string message) {

    if (!isConnected) {
		return;
	}

    json j;

    j["event"] = "log:message";
    j["data"]["id"] = messageID;
    j["data"]["message"] = message;

    // Send message through web socket
    _sendMessage(j.dump());

    messageID++;

}

/// <summary>
/// Adds a guest to the overlay.
/// </summary>
/// <param name="id"></param>
/// <param name="name"></param>
void Overlay::addGuest(string id, string name) {

    if (!isConnected) {
        return;
    }

	json j;

	j["event"] = "guest:add";
	j["data"]["id"] = id;
	j["data"]["name"] = name;

	// Send message through web socket
	_sendMessage(j.dump());

}

/// <summary>
/// Removes a guest from the overlay.
/// </summary>
/// <param name="id"></param>
void Overlay::removeGuest(string id) {

    if (!isConnected) {
        return;
    }

	json j;

	j["event"] = "guest:remove";
	j["data"]["id"] = id;

	// Send message through web socket
	_sendMessage(j.dump());

}

/// <summary>
/// Send latencies to the overlay.
/// </summary>
void Overlay::guestPoll() {

    if (!isConnected) {
        return;
    }

    json j;

    j["event"] = "guest:ping";

    // Users array
    json users = json::array();

	// For each guest in room
	for (Guest guest : g_hosting.getGuests()) {
        
        // Add user to array
        users.push_back({
			{"id", to_string(guest.userID)},
			{"latency", g_hosting.getGuestList().getMetrics(guest.id).metrics.networkLatency }
		});

	}

	j["data"]["users"] = users;

	// Send message through web socket
	_sendMessage(j.dump());

}

/// <summary>
/// Assigns a gamepad to a user.
/// </summary>
/// <param name="userID"></param>
/// <param name="userName"></param>
/// <param name="padID"></param>
void Overlay::padAssign(uint32_t userID, string userName, int padID) {

    if (!isConnected) {
		return;
	}

	json j;

	j["event"] = "gamepad:assign";

	j["data"]["index"] = padID;
    j["data"]["owner"] = {
		{"id", userID},
		{"name", userName}
	};

    // If hotseat is enabled
    j["data"]["hotseatTime"] = Hotseat::instance.getUserTimeRemaining(userID);

	// Send message through web socket
	_sendMessage(j.dump());

}

/// <summary>
/// Clears a pad's owner.
/// </summary>
/// <param name="userID"></param>
void Overlay::padUnassign(uint32_t userID) {

    if (!isConnected) {
		return;
	}

	json j;

	j["event"] = "gamepad:unassign";
	j["data"]["id"] = userID;

	// Send message through web socket
	_sendMessage(j.dump());

}

/// <summary>
/// Send gamepad data to the overlay.
/// </summary>
void Overlay::padPoll() {

    if (!isConnected) {
		return;
	}

	json j;

	j["event"] = "gamepad:poll";

	// Users array
	json pads = json::array();

	// For each gamepad in g_hosting.getGamepads()
    for (AGamepad* gamepad : g_hosting.getGamepads()) {
        if (gamepad->isConnected()) {

            json pad;

            // Owner
            if (gamepad->owner.guest.userID != 0) {
                json owner;
                owner["id"] = gamepad->owner.guest.userID;
                owner["name"] = gamepad->owner.guest.name;

                owner["hotseatTime"] = Hotseat::instance.getUserTimeRemaining(gamepad->owner.guest.userID);

                pad["owner"] = owner;
			}

            // Button array
            WORD wButtons = gamepad->getState().Gamepad.wButtons;
            json buttons = json::array();
            buttons.push_back((wButtons & XUSB_GAMEPAD_A) != 0);
            buttons.push_back((wButtons & XUSB_GAMEPAD_B) != 0);
            buttons.push_back((wButtons & XUSB_GAMEPAD_X) != 0);
            buttons.push_back((wButtons & XUSB_GAMEPAD_Y) != 0);
            buttons.push_back((wButtons & XUSB_GAMEPAD_LEFT_SHOULDER) != 0);
            buttons.push_back((wButtons & XUSB_GAMEPAD_RIGHT_SHOULDER) != 0);
            buttons.push_back(gamepad->getState().Gamepad.bLeftTrigger > 0);
            buttons.push_back(gamepad->getState().Gamepad.bRightTrigger > 0);
            buttons.push_back((wButtons & XUSB_GAMEPAD_BACK) != 0);
            buttons.push_back((wButtons & XUSB_GAMEPAD_START) != 0);
            buttons.push_back((wButtons & XUSB_GAMEPAD_LEFT_THUMB) != 0);
            buttons.push_back((wButtons & XUSB_GAMEPAD_RIGHT_THUMB) != 0);
            buttons.push_back((wButtons & XUSB_GAMEPAD_DPAD_UP) != 0);
            buttons.push_back((wButtons & XUSB_GAMEPAD_DPAD_DOWN) != 0);
            buttons.push_back((wButtons & XUSB_GAMEPAD_DPAD_LEFT) != 0);
            buttons.push_back((wButtons & XUSB_GAMEPAD_DPAD_RIGHT) != 0);

            // Stick values
            ImVec2 leftStick;
            ImVec2 rightStick;
            float lDistance = 0, rDistance = 0;

            leftStick = stickShortToFloat(gamepad->getState().Gamepad.sThumbLX, gamepad->getState().Gamepad.sThumbLY, lDistance);
            rightStick = stickShortToFloat(gamepad->getState().Gamepad.sThumbRX, gamepad->getState().Gamepad.sThumbRY, rDistance);

            json axes = json::array();
            axes.push_back(leftStick.x);
            axes.push_back(leftStick.y);
            axes.push_back(rightStick.x);
            axes.push_back(rightStick.y);

            // Create the pad object
            pad["index"] = gamepad->getIndex();
            pad["buttons"] = buttons;
            pad["axes"] = axes;

            // Add the pad to the array
            pads.push_back(pad);

		}
	}

	j["data"]["gamepads"] = pads;

	// Send message through web socket
	_sendMessage(j.dump());

}

/// <summary>
/// Sends a message to the web socket.
/// </summary>
/// <param name="message"></param>
void Overlay::_sendMessage(string message) {

    if (isConnected) {

        // Send message through web socket
        _client.send(_handle, message, websocketpp::frame::opcode::text);

    }

}

/// <summary>
/// Socket opened successfully event.
/// </summary>
/// <param name="c"></param>
/// <param name="hdl"></param>
void Overlay::_onOpen(client* c, websocketpp::connection_hdl hdl) {
    _handle = hdl;
    isConnected = true;
    g_hosting.logMessage("Connected to overlay!");

    // For each guest in room
    for (Guest guest : g_hosting.getGuests()) {
        addGuest(to_string(guest.userID), guest.name);
	}

    // Lambda thread to send latencies
    processThread = thread([this]() {

		while (isConnected) {
			guestPoll();
            padPoll();
            Sleep(1000 / 60);
		}

	});

	// Start the thread
	processThread.detach();
}

/// <summary>
/// Connected failed event.
/// </summary>
/// <param name="c"></param>
/// <param name="hdl"></param>
void Overlay::_onFail(client* c, websocketpp::connection_hdl hdl) {
    c->get_alog().write(websocketpp::log::alevel::app, "Connection Failed");
}

/// <summary>
/// Message recieved event.
/// </summary>
/// <param name="c"></param>
/// <param name="hdl"></param>
/// <param name="msg"></param>
void Overlay::_onMessage(client* c, websocketpp::connection_hdl hdl, message_ptr msg) {

    json j = json::parse(msg->get_payload());
    if (j["event"] == "chat:message") {

        string message;
        j.at("data").get_to(message);
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
    c->get_alog().write(websocketpp::log::alevel::app, "Connection Closed");
    isConnected = false;
}

/// <summary>
/// Prints message in chat log.
/// </summary>
/// <param name="message"></param>
void Overlay::_log(string message) {

    //_chatLog->logCommand("[WebSocket] " + message);

}

/// <summary>
/// Used to check if the overlay process is running
/// before trying to connect to it.
/// </summary>
/// <returns></returns>
bool Overlay::_isOverlayRunning() {
    // Get the process handle
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, processId);
    if (hProcess != NULL) {
        // Check if the process is still running
        DWORD exitCode;
        if (GetExitCodeProcess(hProcess, &exitCode) && exitCode == STILL_ACTIVE) {
            // Terminate the process
            CloseHandle(hProcess);
            return true;
        }
    }
    // Process is not running
    return false;
}

void Overlay::updateSettings() {

    if (!isConnected) {
        return;
    }

    json j;

    j["event"] = "config:updated";
    j["data"]["chat"]["active"] = Config::cfg.overlay.chat.active;
    j["data"]["chat"]["position"] = Config::cfg.overlay.chat.position;
    j["data"]["chat"]["showHistory"] = Config::cfg.overlay.chat.showHistory;

    j["data"]["gamepads"]["active"] = Config::cfg.overlay.gamepads.active;
    j["data"]["gamepads"]["showHotseat"] = Config::cfg.overlay.gamepads.showHotseatDuration;
    j["data"]["gamepads"]["position"] = Config::cfg.overlay.gamepads.position;

    j["data"]["guests"]["active"] = Config::cfg.overlay.guests.active;
    j["data"]["guests"]["showLatency"] = Config::cfg.overlay.guests.showLatency;
    j["data"]["guests"]["position"] = Config::cfg.overlay.guests.position;

    // Send message through web socket
    _sendMessage(j.dump());

}

/// <summary>
/// Converts a stick value from short to float.
/// </summary>
/// <param name="lx"></param>
/// <param name="ly"></param>
/// <param name="distance"></param>
ImVec2 Overlay::stickShortToFloat(SHORT lx, SHORT ly, float& distance) {
    static float shortMax = 32768;
    ImVec2 stick = ImVec2(
        (float)lx / shortMax,
        (float)ly / shortMax
    );
    stick.x = min(max(stick.x, -1.0f), 1.0f);
    stick.y = min(max(stick.y, -1.0f), 1.0f);

    distance = sqrtf(stick.x * stick.x + stick.y * stick.y);
    if (distance > 0.01f)
    {
        if (abs(lx) > abs(ly))
        {
            if (abs(stick.x) > 0.01f)
            {
                float yy = stick.y / abs(stick.x);
                float xx = 1.0f;
                float maxDiagonal = sqrtf(xx * xx + yy * yy);
                if (maxDiagonal > 0.01f)
                {
                    stick.x /= maxDiagonal;
                    stick.y /= maxDiagonal;
                }
            }
        }
        else
        {
            if (abs(stick.y) > 0.01f)
            {
                float xx = stick.x / abs(stick.y);
                float yy = 1.0f;
                float maxDiagonal = sqrtf(xx * xx + yy * yy);
                if (maxDiagonal > 0.01f)
                {
                    stick.x /= maxDiagonal;
                    stick.y /= maxDiagonal;
                }
            }
        }
    }

    return stick;
}