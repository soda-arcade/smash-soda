#pragma once

#include <iostream>
#include "ACommand.h"
#include "../GamepadClient.h"

class CommandAddXbox : public ACommand
{
public:

	const COMMAND_TYPE type() override { return COMMAND_TYPE::PADS; }

	CommandAddXbox(GamepadClient& gamepadClient)
		: _gamepadClient(gamepadClient)
	{}

	bool run() override {

		_gamepadClient.createGamepad();

		_replyMessage = MetadataCache::preferences.chatbotName + " | Added Xbox controller.\0";
		return true;
	}

	static vector<const char*> prefixes() {
		return vector<const char*> { "!addxbox", "!addxboxpad" };
	}

protected:
	GamepadClient& _gamepadClient;
};