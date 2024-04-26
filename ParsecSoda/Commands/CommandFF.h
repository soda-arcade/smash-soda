#pragma once

#include <iostream>
#include <sstream>
#include "ACommand.h"
#include "../GamepadClient.h"
#include "../Guest.h"

class CommandFF : public ACommand
{
public:
	const COMMAND_TYPE type() override { return COMMAND_TYPE::FF; }

	CommandFF(Guest &sender, GamepadClient &gamepadClient, Hotseat &hotseat)
		: _sender(sender), _gamepadClient(gamepadClient), _droppedPadCount(0), _hotseat(hotseat)
	{}

	bool run() override
	{
		std::ostringstream reply;

		if (Config::cfg.hotseat.enabled) {
			Hotseat::instance.pauseUser(_sender.userID);
		}

		_droppedPadCount = _gamepadClient.onQuit(_sender);
		if (_droppedPadCount > 1) {
			reply << Config::cfg.chatbotName << _sender.name << " has dropped " << _droppedPadCount << " gamepads!\0";
		}
		else if (_droppedPadCount > 0) {
			reply << Config::cfg.chatbotName << _sender.name << " has dropped " << _droppedPadCount << " gamepad!\0";
		}
		else {
			reply << Config::cfg.chatbotName << _sender.name << " has no gamepads to drop.\0";
		}

		_replyMessage = reply.str();
		reply.clear();

		return true;
	}

	int droppedPadCount() {
		return _droppedPadCount;
	}

	static vector<const char*> prefixes() {
		return vector<const char*> { "!ff", "!drop", "!quit" };
	}

protected:
	Guest& _sender;
	GamepadClient &_gamepadClient;
	Hotseat& _hotseat;
	int _droppedPadCount;
};