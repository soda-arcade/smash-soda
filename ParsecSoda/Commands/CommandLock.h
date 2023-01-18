#pragma once

#include <iostream>
#include <sstream>
#include "ACommandIntegerArg.h"
#include "../GamepadClient.h"

class CommandLock : public ACommandIntegerArg
{
public:
	const COMMAND_TYPE type() override { return COMMAND_TYPE::TAKE; }

	CommandLock(const char* msg, Guest& sender, GamepadClient& gamepadClient)
		: ACommandIntegerArg(msg, internalPrefixes()), _sender(sender), _gamepadClient(gamepadClient)
	{}

	bool run() override
	{
		if (!ACommandIntegerArg::run())
		{
			_replyMessage = MetadataCache::preferences.chatbotName + " | Usage: !lock <integer in range [1, 4]>\nExample: !lock 4\0";
			return false;
		}

		
		AGamepad* pad = _gamepadClient.getGamepad(_intArg);
		if (pad) {
			pad->setLocked(!pad->isLocked());

			std::ostringstream reply;
			reply << MetadataCache::preferences.chatbotName + " | Gamepad " << _intArg << " was locked by " << _sender.name << "\0";
			_replyMessage = reply.str();
		}
		return true;
	}

	static vector<const char*> prefixes()
	{
		return vector<const char*> { "!lock" };
	}

protected:
	static vector<const char*> internalPrefixes()
	{
		return vector<const char*> { "!lockpad " };
	}
	string _msg;
	Guest& _sender;
	GamepadClient& _gamepadClient;
};