#pragma once

#include <sstream>
#include "../Base/ACommandIntegerArg.h"
#include "../../../GamepadClient.h"

class CommandDC : public ACommandIntegerArg
{
public:

	/**
	 * @brief Construct a new CommandDC object
	 * 
	 * @param msg
	 * @param gamepadClient
	 */
	CommandDC(const char* msg, GamepadClient& gamepadClient)
		: ACommandIntegerArg(msg, internalPrefixes()), _gamepadClient(gamepadClient)
	{}

	/**
	 * @brief Run the command
	 * 
	 * @return true
	 * @return false
	 */
	bool run() override {
		size_t maxIndex = _gamepadClient.gamepads.size();

		if ( !ACommandIntegerArg::run() ) {
			SetReply("Usage: !dc <integer in range [1, " + to_string(maxIndex) + "]>\nExample: !dc 1\0");
			return false;
		}

		if (_intArg < 1 || _intArg > maxIndex) {
			SetReply("Wrong index: " + to_string(_intArg) + " is not in range [1, " + to_string(maxIndex) + "].\0");
		}

		if (_gamepadClient.disconnect(_intArg - 1)) {
			SetReply("Gamepad " + to_string(_intArg) + " disconnected.\0");
		} else{
			SetReply("Gamepad " + to_string(_intArg) + " fail to disconnect.\0");
		}
		return true;
	}

	/**
	 * @brief Get the prefixes object
	 * 
	 * @return std::vector<const char*>
	 */
	static vector<const char*> prefixes() {
		return vector<const char*> { "!dc" };
	}

private:
	static vector<const char*> internalPrefixes() {
		return vector<const char*> { "!dc " };
	}

	GamepadClient &_gamepadClient;

};