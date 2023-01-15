#pragma once

#include <sstream>
#include "parsec-dso.h"
#include "ACommandIntegerArg.h"

class CommandSpot : public ACommandIntegerArg
{
public:
	const COMMAND_TYPE type() override { return COMMAND_TYPE::GUESTS; }

	CommandSpot(const char* msg, ParsecHostConfig& config)
		: ACommandIntegerArg(msg, internalPrefixes()), _config(config)
	{}

	bool run() override
	{
		if (!ACommandIntegerArg::run())
		{
			_replyMessage = MetadataCache::preferences.chatbotName + " | Usage: !spot <number>\nExample: !spot 1\0";
			return false;
		}

		_config.maxGuests += _intArg;
		MetadataCache::preferences.roomChanged = true;
		std::ostringstream reply;
		reply << MetadataCache::preferences.chatbotName + " | Added " << _intArg << " guest slot(s)\0";
		_replyMessage = reply.str();

		return true;
	}

	static vector<const char*> prefixes()
	{
		return vector<const char*> { "!spot" };
	}

protected:
	static vector<const char*> internalPrefixes()
	{
		return vector<const char*> { "!spot " };
	}

	ParsecHostConfig& _config;
};