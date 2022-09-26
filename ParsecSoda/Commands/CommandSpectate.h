#pragma once

#include "ACommand.h"
#include <iostream>
#include <algorithm>
#include "../CompilerDirectives.h"

using namespace std;

class CommandSpectate : public ACommand
{
public:
	const COMMAND_TYPE type() override { return COMMAND_TYPE::BOT_MESSAGE; }

	CommandSpectate(Guest& sender)
		: _sender(sender)
	{}

	bool run() override {

		if (MetadataCache::hotseat.spectators.empty() == false) {
			for (int i = MetadataCache::hotseat.spectators.size() - 1; i >= 0; i--) {
				if (MetadataCache::hotseat.spectators.at(i) == _sender.userID) {
					MetadataCache::hotseat.spectators.erase(MetadataCache::hotseat.spectators.begin() + i);
					_replyMessage = MetadataCache::preferences.chatbotName + " | " + _sender.name + " is no longer spectating.\0";
					return true;
				}
			}
		}
		
		_replyMessage = MetadataCache::preferences.chatbotName + " | " + _sender.name + " is now spectating.\0";
		MetadataCache::hotseat.spectators.push_back(_sender.userID);
		return true;
	}

	static vector<const char*> prefixes()
	{
		return vector<const char*> { "!spectate", "!spectating", "!watching", "!skipme" };
	}

protected:
	Guest& _sender;
};