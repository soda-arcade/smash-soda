#pragma once

#include <sstream>
#include "parsec-dso.h"
#include "ACommandIntegerArg.h"

class CommandExtend : public ACommandIntegerArg
{
public:
	const COMMAND_TYPE type() override { return COMMAND_TYPE::BOT_MESSAGE; }

	CommandExtend(const char* msg)
		: ACommandIntegerArg(msg, internalPrefixes())
	{}

	bool run() override {

		if (!MetadataCache::preferences.hotseat) return false;

		if (!ACommandIntegerArg::run()) {
			_replyMessage = MetadataCache::preferences.chatbotName + " | Usage: !extend 5 7\0";
			return false;
		}

		// Add time to timer
		uint32_t minutes = MetadataCache::hotseat.hotseatClock.getDuration();
		minutes += _intArg * 60000;
		MetadataCache::hotseat.hotseatClock.setDuration(minutes);

		std::ostringstream reply;
		reply << "[HOTSEAT] | The hotseat time for current guest extended by: " << _intArg << " minutes.\0";
		_replyMessage = reply.str();

		return true;

	}

	static vector<const char*> prefixes() {
		return vector<const char*> { "!extend" };
	}

protected:
	static vector<const char*> internalPrefixes() {
		return vector<const char*> { "!extend " };
	}
};