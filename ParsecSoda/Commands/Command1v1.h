#pragma once

#include <iostream>
#include <sstream>
#include "ACommandIntegerArg.h"
#include "../GuestList.h"
#include "../GamepadClient.h"

class Command1v1 : public ACommandIntegerArg
{
public:
	const COMMAND_TYPE type() override { return COMMAND_TYPE::BOT_MESSAGE; }

	Command1v1(const char* msg, GuestList& guestList, GamepadClient& gamepadClient)
		: ACommandIntegerArg(msg, internalPrefixes()), _guestList(guestList), _gamepadClient(gamepadClient)
	{}

	bool run() override
	{
		if (!ACommandIntegerArg::run())
		{
			_replyMessage = MetadataCache::preferences.chatbotName + " | Usage: !1v1\0";
			return false;
		}

		_replyMessage = "\0";
		return true;
	}

	static vector<const char*> prefixes()
	{
		return vector<const char*> { "!1v1" };
	}

protected:
	static vector<const char*> internalPrefixes()
	{
		return vector<const char*> { "!1v1 " };
	}
	string _msg;
	GuestList& _guestList;
	GamepadClient& _gamepadClient;
};