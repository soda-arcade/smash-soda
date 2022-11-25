#pragma once

#include <sstream>
#include <algorithm>
#include "parsec-dso.h"
#include "ACommandIntegerArg.h"

class CommandDecrease : public ACommandIntegerArg
{
public:
	const COMMAND_TYPE type() override { return COMMAND_TYPE::BOT_MESSAGE; }

	CommandDecrease(const char* msg)
		: ACommandIntegerArg(msg, internalPrefixes())
	{}

	bool run() override {

		if (!MetadataCache::preferences.hotseat) return false;

		if (!ACommandIntegerArg::run()) {
			_replyMessage = MetadataCache::preferences.chatbotName + " | Usage: !decrease 5 7\0";
			return false;
		}

		// Add time to timer
		uint32_t minutes = MetadataCache::hotseat.hotseatClock.getDuration();
		uint32_t decreaseBy = _intArg * 60000;

		if ((minutes - decreaseBy) > 0) {

			MetadataCache::hotseat.hotseatClock.setDuration(minutes - decreaseBy);

		}
		else {

			MetadataCache::hotseat.hotseatClock.setDuration(0);

		}

		std::ostringstream reply;
		reply << "[HOTSEAT] | The hotseat time for current guest was decreased by: " << _intArg << " minutes.\0";
		_replyMessage = reply.str();

		return true;

	}

	static vector<const char*> prefixes() {
		return vector<const char*> { "!decrease" };
	}

protected:
	static vector<const char*> internalPrefixes() {
		return vector<const char*> { "!decrease " };
	}
};