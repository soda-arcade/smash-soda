#pragma once

#include "ACommand.h"
#include <iostream>
#include "../CompilerDirectives.h"

using namespace std;

class CommandHotseat : public ACommand
{
public:
	const COMMAND_TYPE type() override { return COMMAND_TYPE::SETCONFIG; }

	CommandHotseat(Guest& sender)
		: _sender(sender)
	{}

	bool run() override {

		if (MetadataCache::preferences.hotseat) {
			_replyMessage = MetadataCache::preferences.chatbotName + " hotseat has been disabled.";
			MetadataCache::preferences.hotseat = false;
		}
		else {
			_replyMessage = MetadataCache::preferences.chatbotName + " hotseat has been enabled.";
			MetadataCache::preferences.hotseat = true;
		}

		return true;

	}

	static vector<const char*> prefixes() {
		return vector<const char*> { "!hotseat", "!hotseatoff", "!hotseaton" };
	}

protected:
	Guest& _sender;
};