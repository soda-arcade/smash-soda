#pragma once

#include <iostream>
#include "ACommand.h"
#include "../GamepadClient.h"

class CommandAddPS : public ACommand
{
public:

	const COMMAND_TYPE type() override { return COMMAND_TYPE::PADS; }

	CommandAddPS(GamepadClient& gamepadClient)
		: _gamepadClient(gamepadClient)
	{}

	bool run() override {

		_gamepadClient.createGamepad(AGamepad::Type::DUALSHOCK);

		_replyMessage = MetadataCache::preferences.chatbotName + " | Added playstation controller.\0";
		return true;
	}

	static vector<const char*> prefixes() {
		return vector<const char*> { "!addps", "!addds", "!addpspad", "!adddspad" };
	}

protected:
	GamepadClient& _gamepadClient;
};