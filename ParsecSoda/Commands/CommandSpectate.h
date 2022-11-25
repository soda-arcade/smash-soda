#pragma once

#include "ACommandSearchUser.h"
#include <iostream>
#include <algorithm>
#include "../TierList.h"
#include "../CompilerDirectives.h"

using namespace std;

class CommandSpectate : public ACommandSearchUser
{
public:
	const COMMAND_TYPE type() override { return COMMAND_TYPE::KICK; }

	CommandSpectate(const char* msg, Guest& sender, GuestList& guests, TierList& tierList)
		: ACommandSearchUser(msg, internalPrefixes(), guests), _sender(sender), _tierList(tierList)
	{}

	bool run() override {

		ACommandSearchUser::run();
		
		switch (_searchResult)
		{
		case SEARCH_USER_RESULT::NOT_FOUND:
			if (_tierList.getTier(_sender.userID) != Tier::PLEB) {
				_replyMessage = std::string() + MetadataCache::preferences.chatbotName + " | " + _sender.name + ", I cannot find the user you want to set to spectator.\0";
			}
			break;

		case SEARCH_USER_RESULT::FOUND:

			if (_tierList.getTier(_sender.userID) != Tier::PLEB) {
				addGuestToSpectators(_targetGuest);
			}
			
			break;
		
		case SEARCH_USER_RESULT::FAILED:
		default:
			
			addGuestToSpectators(_sender);

			break;
		}

		return true;
	}

	bool addGuestToSpectators(Guest guest) {

		if (MetadataCache::hotseat.spectators.empty() == false) {
			for (int i = MetadataCache::hotseat.spectators.size() - 1; i >= 0; i--) {
				if (MetadataCache::hotseat.spectators.at(i) == guest.userID) {
					MetadataCache::hotseat.spectators.erase(MetadataCache::hotseat.spectators.begin() + i);
					MetadataCache::hotseat.spectators.shrink_to_fit();
					_replyMessage = MetadataCache::preferences.chatbotName + " | " + guest.name + " is no longer spectating.\0";
					return true;
				}
			}
		}

		_replyMessage = MetadataCache::preferences.chatbotName + " | " + guest.name + " is now spectating.\0";
		MetadataCache::hotseat.spectators.push_back(guest.userID);
		return true;

	}

	static vector<const char*> prefixes()
	{
		return vector<const char*> { "!spectate" };
	}

protected:
	static vector<const char*> internalPrefixes()
	{
		return vector<const char*> { "!spectate " };
	}

	Guest& _sender;
	TierList& _tierList;
};