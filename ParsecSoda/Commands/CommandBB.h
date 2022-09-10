#pragma once

#include <iostream>
#include "ACommand.h"
#include "../GamepadClient.h"

class CommandBB : public ACommand
{
public:

	const COMMAND_TYPE type() override { return COMMAND_TYPE::PADS; }

	CommandBB(GamepadClient& gamepadClient)
		: _gamepadClient(gamepadClient)
	{}

	bool run() override {

		MetadataCache::preferences.buttonList.push_back(ParsecGamepadButton::GAMEPAD_BUTTON_B);
		MetadataCache::preferences.buttonList.push_back(ParsecGamepadButton::GAMEPAD_BUTTON_B);

		_replyMessage = "[ChatBot] | Everybody's B button was pressed twice!\0";
		return true;
	}

	static vector<const char*> prefixes() {
		return vector<const char*> { "!bb", "!bbb" };
	}

protected:
	GamepadClient& _gamepadClient;
};