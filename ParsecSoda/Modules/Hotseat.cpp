#include "Hotseat.h"

/// <summary>
/// Constructor
/// </summary>
Hotseat::Hotseat() : _guestList(nullptr), _gamepadClient(nullptr) {}

/// <summary>
/// Initialization
/// </summary>
/// <param name="gamepadClient"></param>
void Hotseat::init(GuestList& guestList, GamepadClient& gamepadClient) {
	_guestList = &guestList;
	_gamepadClient = &gamepadClient;
}

/// <summary>
/// Handles the hotseat swapping
/// </summary>
void Hotseat::run() {

}

/// <summary>
/// Performs various checks to see if the hotseat system can be run
/// </summary>
/// <returns></returns>
bool Hotseat::test() {

}

/// <summary>
/// Formats a status message.
/// </summary>
/// <param name="message">The status message</param>
/// <returns>Formatted message</returns>
string Hotseat::msg(string message) {

	return "[HOTSEAT] " + message;

}