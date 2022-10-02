#pragma once

#include "parsec-dso.h"
#include "ACommandStringArg.h"

class CommandSong : public ACommandStringArg
{
public:
	const COMMAND_TYPE type() override { return COMMAND_TYPE::BOT_MESSAGE; }

	CommandSong(const char* msg)
		: ACommandStringArg(msg, internalPrefixes())
	{}

	bool run() override {

		_replyMessage = MetadataCache::preferences.chatbotName + " | Test!\0";
		return true;

	}

	static vector<const char*> prefixes() {
		return vector<const char*> { "!request", "!songrequest" };
	}

protected:
	static vector<const char*> internalPrefixes() {
		return vector<const char*> { "!request " };
	}
};