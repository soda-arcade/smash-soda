#pragma once

#include "../Base/ACommand.h"
#include "../../../GamepadClient.h"

class CommandOne : public ACommand
{
public:

	/**
	 * @brief Construct a new CommandOne object
	 * 
	 * @param sender
	 * @param gamepadClient
	 */
	CommandOne(Guest &sender, GamepadClient &gamepadClient)
		: _sender(sender), _gamepadClient(gamepadClient)
	{}

	/**
	 * @brief Run the command
	 * 
	 * @return true
	 * @return false
	 */
	bool run() override {
		bool isOne = _gamepadClient.toggleIgnoreDeviceID(_sender.userID);
		SetReply(_sender.name + " toggled ignore device ID mode:\t" + (isOne ? "ON" : "OFF"));
		return true;
	}

	/**
	 * @brief Get the prefixes object
	 * 
	 * @return std::vector<const char*>
	 */
	static vector<const char*> prefixes() {
		return vector<const char*> { "!one" };
	}

protected:
	Guest& _sender;
	GamepadClient& _gamepadClient;
};
