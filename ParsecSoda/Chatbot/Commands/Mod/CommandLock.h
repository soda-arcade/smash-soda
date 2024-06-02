#pragma once

#include "../Base/ACommandIntegerArg.h"
#include "../../../GamepadClient.h"

class CommandLock : public ACommandIntegerArg
{
public:

	/**
	 * @brief Construct a new CommandLock object
	 * 
	 * @param msg
	 * @param sender
	 * @param gamepadClient
	 */
	CommandLock(const char* msg, Guest& sender, GamepadClient& gamepadClient)
		: ACommandIntegerArg(msg, internalPrefixes()), _sender(sender), _gamepadClient(gamepadClient)
	{}

	/**
	 * @brief Run the command
	 * 
	 * @return true
	 * @return false
	 */
	bool run() override {
		if (!ACommandIntegerArg::run()) {
			SetReply("Usage: !lock <integer in range [1, 4]>\nExample: !lock 4\0");
			return false;
		}

		
		AGamepad* pad = _gamepadClient.getGamepad(_intArg);
		if (pad) {
			pad->setLocked(!pad->isLocked());

			std::ostringstream reply;
			SetReply("Gamepad " + std::to_string(_intArg) + " was locked by " + _sender.name + "\0");
		}
		return true;
	}

	/**
	 * @brief Get the prefixes object
	 * 
	 * @return std::vector<const char*>
	 */
	static vector<const char*> prefixes() {
		return vector<const char*> { "!lock" };
	}

protected:
	static vector<const char*> internalPrefixes()
	{
		return vector<const char*> { "!lock " };
	}
	string _msg;
	Guest& _sender;
	GamepadClient& _gamepadClient;
};