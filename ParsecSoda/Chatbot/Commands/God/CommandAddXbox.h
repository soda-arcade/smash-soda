#pragma once

#include <iostream>
#include "../Base/ACommand.h"
#include "../../../GamepadClient.h"

class CommandAddXbox : public ACommand
{
public:

	CommandAddXbox(GamepadClient& gamepadClient)
		: _gamepadClient(gamepadClient)
	{}

	bool run() override {

		_gamepadClient.createGamepad(AGamepad::Type::XBOX, true);
		SetReply("Added xbox controller.\0");
		return true;
	}

	static vector<const char*> prefixes() {
		return vector<const char*> { "!addxbox", "!addxboxpad" };
	}

protected:
	GamepadClient& _gamepadClient;
};