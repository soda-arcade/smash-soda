#pragma once

#include <iostream>
#include "ACommand.h"
#include "../GamepadClient.h"
#include "../Modules/Macro.h"
#include "../TierList.h"
#include "../VipList.h"

class CommandLockAll : public ACommand
{
public:

	const COMMAND_TYPE type() override { return COMMAND_TYPE::PADS; }

	CommandLockAll(GamepadClient& gamepadClient)
		: _gamepadClient(gamepadClient)
	{}

	bool run() override {

		if (Config::cfg.input.lockedPads) {

			std::vector<AGamepad*>::iterator gi = _gamepadClient.gamepads.begin();
			for (; gi != _gamepadClient.gamepads.end(); ++gi) {
				(*gi)->setLocked(false);
			}
			Config::cfg.input.lockedPads = false;
			_replyMessage = Config::cfg.chatbotName + "Everybody's pads were unlocked!\0";

		}
		else {

			std::vector<AGamepad*>::iterator gi = _gamepadClient.gamepads.begin();
			for (; gi != _gamepadClient.gamepads.end(); ++gi) {
				(*gi)->setLocked(true);
			}
			Config::cfg.input.lockedPads = true;
			_replyMessage = Config::cfg.chatbotName + "Everybody's pads were locked!\0";

		}

		return true;

	}

	static vector<const char*> prefixes() {
		return vector<const char*> { "!lockall", "!unlockall", "!lockpads", "!unlockpads" };
	}

protected:
	GamepadClient& _gamepadClient;
};