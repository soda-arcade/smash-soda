#include "WebSocket.h"

/// <summary>
/// Constructor
/// </summary>
WebSocket::WebSocket() : _chatLog(nullptr) {}

/// <summary>
/// Initialization
/// </summary>
/// <param name="gamepadClient"></param>
void WebSocket::init(ChatLog& chatLog) {
	_chatLog = &chatLog;
}

/// <summary>
/// Creates a new websocket.
/// </summary>
/// <returns>True if successful</returns>
bool WebSocket::createSocket() {

	return false;

}

/// <summary>
/// Loops through response from server and stores in _response.
/// </summary>
void WebSocket::getData() {

}