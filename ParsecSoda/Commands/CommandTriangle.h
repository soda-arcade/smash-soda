#pragma once

#include "ACommand.h"
#include <iostream>
#include "../CompilerDirectives.h"

using namespace std;

class CommandTriangle : public ACommand
{
public:
	const COMMAND_TYPE type() override { return COMMAND_TYPE::BOT_MESSAGE; }

	CommandTriangle(Guest& sender, GamepadClient& gamepadClient, Macro& macro)
		: _sender(sender), _gamepadClient(gamepadClient), _macro(macro)
	{}

	bool run() override {

		// Random seed from time
		srand(time(NULL));

		int randomNumber = 1 + (rand() % 10);
		
		ostringstream reply;

		reply << MetadataCache::preferences.chatbotName + " " + _sender.name + 
			" pressed the triangle button ";
		reply << randomNumber;
		reply << " times!";

		/*for (int i = 0; i < randomNumber; i++) {
			_macro.pressButtonForGuest(_sender.userID, ParsecGamepadButton::GAMEPAD_BUTTON_Y);
		}*/

		_replyMessage = reply.str();

		return true;

	}

	static vector<const char*> prefixes() {
		return vector<const char*> { "!triangle" };
	}

protected:
	Guest& _sender;
	GamepadClient& _gamepadClient;
	Macro& _macro;
};