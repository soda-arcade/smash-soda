#pragma once

#include <sstream>
#include "ACommandIntegerArg.h"
#include "../GamepadClient.h"

class CommandRC : public ACommandIntegerArg
{
public:
	const COMMAND_TYPE type() override { return COMMAND_TYPE::DC; }

	CommandRC(const char* msg, GamepadClient& gamepadClient)
		: ACommandIntegerArg(msg, internalPrefixes()), _gamepadClient(gamepadClient)
	{}

	bool run() override
	{
		size_t maxIndex = _gamepadClient.gamepads.size();

		if (!ACommandIntegerArg::run())
		{
			_replyMessage =
				std::string() +
				MetadataCache::preferences.chatbotName + " | Usage: !rc <integer in range [1, " +
				to_string(maxIndex) +
				"]>\nExample: !rc 1\0"
				;
			return false;
		}


		std::ostringstream reply;
		if (_intArg < 1 || _intArg > maxIndex)
		{
			reply << MetadataCache::preferences.chatbotName + " | Wrong index: " << _intArg << " is not in range [1, " << maxIndex << "].\0";
		}

		if (_gamepadClient.connect(_intArg - 1))
		{
			reply << MetadataCache::preferences.chatbotName + " | Gamepad " << _intArg << " connected.\0";
		}
		else
		{
			reply << MetadataCache::preferences.chatbotName + " | Gamepad " << _intArg << " fail to connect.\0";
		}

		_replyMessage = reply.str();
		return true;
	}

	static vector<const char*> prefixes()
	{
		return vector<const char*> { "!rc" };
	}

private:
	static vector<const char*> internalPrefixes()
	{
		return vector<const char*> { "!rc " };
	}

	GamepadClient& _gamepadClient;

};