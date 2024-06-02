#pragma once

#include "../Base/ACommandIntegerArg.h"
#include "../../../GamepadClient.h"

class CommandRC : public ACommandIntegerArg
{
public:

	/**
	 * @brief Construct a new CommandRC object
	 * 
	 * @param msg
	 * @param gamepadClient
	 */
	CommandRC(const char* msg, GamepadClient& gamepadClient)
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

		if (!ACommandIntegerArg::run()) {
			SetReply("Usage: !rc <integer in range [1, " + to_string(maxIndex) + "]>\nExample: !rc 1\0");
			return false;
		}

		std::ostringstream reply;
		if (_intArg < 1 || _intArg > maxIndex) {
			SetReply("Wrong index: " + to_string(_intArg) + " is not in range [1, " + to_string(maxIndex) + "].\0");
		}

		if (_gamepadClient.connect(_intArg - 1)) {
			SetReply("Gamepad " + to_string(_intArg) + " connected.\0");
		} else {
			SetReply("Gamepad " + to_string(_intArg) + " failed to connect.\0");
		}

		_replyMessage = reply.str();
		return true;
	}

	/**
	 * @brief Get the prefixes object
	 * 
	 * @return std::vector<const char*>
	 */
	static vector<const char*> prefixes() {
		return vector<const char*> { "!rc" };
	}

private:
	static vector<const char*> internalPrefixes()
	{
		return vector<const char*> { "!rc " };
	}

	GamepadClient& _gamepadClient;

};