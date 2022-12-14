#pragma once

#include <sstream>
#include "parsec-dso.h"
#include "ACommandIntegerArg.h"

class CommandSpot : public ACommandIntegerArg
{
public:
	const COMMAND_TYPE type() override { return COMMAND_TYPE::GUESTS; }

	CommandSpot(const char* msg, ParsecDSO* parsec, ParsecHostConfig& config, const char* sessionId)
		: ACommandIntegerArg(msg, internalPrefixes()), _config(config), _sessionId(sessionId)
	{}

	bool run() override
	{
		if (!ACommandIntegerArg::run())
		{
			_replyMessage = MetadataCache::preferences.chatbotName + " | Usage: !spot <number>\nExample: !spot 1\0";
			return false;
		}

		_config.maxGuests = _config.maxGuests + 1;
		ParsecHostSetConfig(_parsec, &_config, _sessionId);
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

	ParsecDSO* _parsec;
	ParsecHostConfig& _config;
	const char* _sessionId;
};
