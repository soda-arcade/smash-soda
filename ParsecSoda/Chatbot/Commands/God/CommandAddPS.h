#pragma once

#include <iostream>
#include "../Base/ACommand.h"
#include "../../../GamepadClient.h"

class CommandAddPS : public ACommand
{
public:

	/**
	 * @brief Construct a new CommandAddPS object
	 * 
	 * @param gamepadClient The gamepad client
	 */
	CommandAddPS(GamepadClient& gamepadClient)
		: _gamepadClient(gamepadClient)
	{}

	/**
	 * @brief Run the command
	 * @return true if the command was successful
	 */
	bool run() override {
		_gamepadClient.createGamepad(AGamepad::Type::DUALSHOCK, true);
		SetReply("Added playstation controller.\0");
		return true;
	}

	/**
	 * @brief Get the prefixes
	 * @return The prefixes
	 */
	static vector<const char*> prefixes() {
		return vector<const char*> { "!addps", "!addds", "!addpspad", "!adddspad" };
	}

protected:
	GamepadClient& _gamepadClient;
};