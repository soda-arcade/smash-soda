#include "Overlay.h"

/// <summary>
/// Constructor
/// </summary>
Overlay::Overlay() : _processMan(nullptr), _webSocket(nullptr), _chatLog(nullptr) {}

/// <summary>
/// Initialization
/// </summary>
/// <param name="gamepadClient"></param>
void Overlay::init(ProcessMan& processMan, WebSocket& webSocket, ChatLog& chatLog) {
	_processMan = &processMan;
	_webSocket = &webSocket;
	_chatLog = &chatLog;
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

			// Start overlay application
			if (_launchOverlay()) {

				isActive = true;
				
				// Sleep for 5 seconds
				Sleep(5000);

				// Open web socket with overlay
				if (_openSocket()) {

					_log("The overlay has been started. Ctrl+Shift+C toggles host chat box for chatting with your guests.");

				}
				else {

					_log("A connection with the overlay could not be made.");

				}

			} else {
		
				_log("The overlay application could not be started.");
		
			}

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

	}

}

/// <summary>
/// Stops the overlay.
/// </summary>
void Overlay::stop() {

	// Only if it had been started successfully
	if (isActive) {

		_processMan->kill(_pid);

		// Stop loop
		isActive = false;

	}

}

/// <summary>
/// Launches the overlay application.
/// </summary>
bool Overlay::_launchOverlay() {

	// Get relative path
	wchar_t buffer[MAX_PATH];
	GetModuleFileNameW(NULL, buffer, MAX_PATH);
	std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
	std::wstring path = std::wstring(buffer).substr(0, pos);
	path += L"\\overlay\\Smash-Soda-Overlay.exe";
	std::string pathStr = std::string(path.begin(), path.end());

	_pid = _processMan->start(pathStr, "");
	if (_pid == 0) {
		return false;
	}

	return true;

}

/// <summary>
/// Opens a web socket connection with the overlay.
/// </summary>
bool Overlay::_openSocket() {

	if (_webSocket->createConnection()) {
		return true;
	}
	else {
		return false;
	}

}

/// <summary>
/// Helper function for logging a message to the chat log.
/// </summary>
void Overlay::_log(string message) {

	_chatLog->logCommand("[Overlay] " + message);

}