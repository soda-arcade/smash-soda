#pragma once

#include "../Base/ACommandIntegerArg.h"
#include "../../../GamepadClient.h"

class CommandStrip : public ACommandIntegerArg
{
public:

	/**
	 * @brief Construct a new CommandStrip object
	 * 
	 * @param msg
	 * @param sender
	 * @param gamepadClient
	 * @param hotseat
	 */
	CommandStrip(const char* msg, Guest& sender, GamepadClient& gamepadClient, Hotseat& hotseat)
		: ACommandIntegerArg(msg, internalPrefixes()), _sender(sender), _gamepadClient(gamepadClient), _hotseat(hotseat)
	{}

	/**
	 * @brief Run the command
	 * 
	 * @return true
	 * @return false
	 */
	bool run() override {

		if (!ACommandIntegerArg::run()) {
			SetReply("Usage: !strip <integer in range [1, 4]>\nExample: !strip 4\0");
			return false;
		}

		bool success = _gamepadClient.clearOwner(_intArg - 1);
		if (!success) {
			SetReply("Usage: !strip <integer in range [1, 4]>\nExample: !strip 4\0");
			return false;
		}

		SetReply("Gamepad " + to_string(_intArg) + " was forcefully dropped by " + _sender.name + "!\0");
		return true;
	}

	/**
	 * @brief Get the prefixes object
	 * 
	 * @return vector<const char*>
	 */
	static vector<const char*> prefixes() {
		return vector<const char*> { "!strip" };
	}

protected:
	static vector<const char*> internalPrefixes()
	{
		return vector<const char*> { "!strip " };
	}
	string _msg;
	Guest& _sender;
	GamepadClient& _gamepadClient;
	Hotseat& _hotseat;
};

