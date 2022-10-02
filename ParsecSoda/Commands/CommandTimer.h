#pragma once

#include <iostream>
#include <sstream>
#include "ACommandIntegerArg.h"
#include "../GamepadClient.h"

class CommandTimer : public ACommandIntegerArg
{
public:
	const COMMAND_TYPE type() override { return COMMAND_TYPE::BOT_MESSAGE; }

	CommandTimer(const char* msg)
		: ACommandIntegerArg(msg, internalPrefixes())
	{}

	bool run() override {

		if (!ACommandIntegerArg::run()) {
			_replyMessage = MetadataCache::preferences.chatbotName + " | Usage: !timer <minutes>\nExample: !timer 4\0";
			return false;
		}

		// Set the timer
		MetadataCache::hotseat.customClock.reset(_intArg);

		std::ostringstream reply;
		reply << MetadataCache::preferences.chatbotName + " | A timer was set for " << _intArg << " minutes.\n";
		_replyMessage = reply.str();

		return true;

	}

	static vector<const char*> prefixes()
	{
		return vector<const char*> { "!timer" };
	}

protected:
	static vector<const char*> internalPrefixes() {
		return vector<const char*> { "!timer " };
	}
	string _msg;
};