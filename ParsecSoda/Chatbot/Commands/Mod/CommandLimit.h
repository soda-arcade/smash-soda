#pragma once

#include "../Base/ACommandSearchUserIntArg.h"
#include "../../../GamepadClient.h"

class CommandLimit : public ACommandSearchUserIntArg
{
public:

	/**
	 * @brief Construct a new CommandLimit object
	 * 
	 * @param msg
	 * @param guests
	 * @param gamepadClient
	 */
	CommandLimit(const char* msg, GuestList &guests, GamepadClient &gamepadClient)
		: ACommandSearchUserIntArg(msg, internalPrefixes(), guests), _gamepadClient(gamepadClient)
	{}

	/**
	 * @brief Run the command
	 * 
	 * @return true
	 * @return false
	 */
	bool run() override {
		if ( !ACommandSearchUserIntArg::run() ) {
			SetReply("Usage: !limit <username> <number>\nExample: !limit melon 2");
			return false;
		}

		_gamepadClient.setLimit(_targetGuest.userID, _intArg);
		SetReply("Gamepad limit set to " + to_string(_intArg) + " for " + _targetGuest.name);
		return true;
	}

	/**
	 * @brief Get the prefixes object
	 * 
	 * @return std::vector<const char*>
	 */
	static vector<const char*> prefixes() {
		return vector<const char*> { "!limit" };
	}

protected:

	/**
	 * @brief Get the internal prefixes object
	 * 
	 * @return std::vector<const char*>
	 */
	static vector<const char*> internalPrefixes() {
		return vector<const char*> { "!limit " };
	}

	GamepadClient& _gamepadClient;
};
