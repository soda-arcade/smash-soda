#pragma once

#include <iostream>
#include <sstream>
#include "ACommandIntegerArg.h"
#include "../GuestList.h"
#include "../GamepadClient.h"

class Command2v2 : public ACommandIntegerArg
{
public:
	const COMMAND_TYPE type() override { return COMMAND_TYPE::BOT_MESSAGE; }

	Command2v2(const char* msg, GuestList& guestList, GamepadClient& gamepadClient)
		: ACommandIntegerArg(msg, internalPrefixes()), _guestList(guestList), _gamepadClient(gamepadClient)
	{}

	bool run() override
	{
		if (!ACommandIntegerArg::run())
		{
			_replyMessage = MetadataCache::preferences.chatbotName + " | Usage: !2v2\0";
			return false;
		}

		_replyMessage = "\0";
		return true;
	}

	static vector<const char*> prefixes()
	{
		return vector<const char*> { "!2v2" };
	}

protected:
	static vector<const char*> internalPrefixes()
	{
		return vector<const char*> { "!2v2 " };
	}
	string _msg;
	GuestList& _guestList;
	GamepadClient& _gamepadClient;
};