#pragma once

#include "ACommand.h"
#include "../GamepadClient.h"
#include <iostream>

class CommandBB : public ACommand
{
public:
	const COMMAND_TYPE type() override { return COMMAND_TYPE::PADS; }
	bool timer;
	int time;
	int presses;
	bool pressed;

	CommandBB(GamepadClient& gamepadClient)
		: _gamepadClient(gamepadClient)
	{}

	bool run() override {

		_replyMessage = "[ChatBot] | Everybody's B button was pressed twice!\0";
		return true;
	}

	static vector<const char*> prefixes() {
		return vector<const char*> { "!bb", "!bbb" };
	}

	void init();
	void pressButton(ParsecGamepadButtonMessage button);
	void pressB();
	void releaseB();

protected:
	GamepadClient& _gamepadClient;
};