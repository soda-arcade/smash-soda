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

		if (MetadataCache::preferences.spectators.empty() == false) {
			for (int i = MetadataCache::preferences.spectators.size() - 1; i >= 0; i--) {
				if (MetadataCache::preferences.spectators.at(i) == _sender.userID) {
					MetadataCache::preferences.spectators.erase(MetadataCache::preferences.spectators.begin() + i);
					_replyMessage = "[MasterHand] | " + _sender.name + " is no longer spectating.\0";
					return true;
				}
			}
		}
		
		_replyMessage = "[MasterHand] | " + _sender.name + " is now spectating.\0";
		MetadataCache::preferences.spectators.push_back(_sender.userID);
		return true;
	}

	static vector<const char*> prefixes()
	{
		return vector<const char*> { "!spectate", "!spectating", "!watching", "!skipme" };
	}

protected:
	Guest& _sender;
};