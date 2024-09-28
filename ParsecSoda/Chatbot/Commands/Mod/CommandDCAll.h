#pragma once

#include "../Base/ACommand.h"
#include "../../../GamepadClient.h"

class CommandDCAll : public ACommand
{
public:

	/**
	 * @brief Construct a new CommandLockAll object
	 *
	 * @param gamepadClient
	 */
	CommandDCAll(GamepadClient& gamepadClient)
		: _gamepadClient(gamepadClient)
	{}

	/**
	 * @brief Run the command
	 *
	 * @return true
	 * @return false
	 */
	bool run() override {

		std::vector<AGamepad*>::iterator gi = _gamepadClient.gamepads.begin();
		for (; gi != _gamepadClient.gamepads.end(); ++gi) {
			if ((*gi)->isConnected()) {
				(*gi)->disconnect();
			}
			else {
				(*gi)->connect();
			}
		}

		return true;

	}

	static vector<const char*> prefixes() {
		return vector<const char*> { "!dcall" };
	}

protected:
	GamepadClient& _gamepadClient;
};