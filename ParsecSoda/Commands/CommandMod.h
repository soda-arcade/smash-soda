#pragma once

#include "ACommandSearchUserHistory.h"
#include <iostream>
#include <Windows.h>
#include <mmsystem.h>
#include "parsec-dso.h"
#include "../ModList.h"

using namespace std;

class CommandMod : public ACommandSearchUserHistory
{
public:
	const COMMAND_TYPE type() override { return COMMAND_TYPE::MOD; }

	CommandMod(const char* msg, Guest& sender, ParsecDSO* parsec, GuestList& guests, GuestDataList& guestHistory, ModList& modList, TierList& tierList)
		: ACommandSearchUserHistory(msg, internalPrefixes(), guests, guestHistory), _sender(sender), _parsec(parsec), _mod(modList), _tierList(tierList)
	{
	}

	bool run() override
	{
		ACommandSearchUserHistory::run();

		bool rv = false;

		switch (_searchResult)
		{
		case SEARCH_USER_HISTORY_RESULT::NOT_FOUND:
			_replyMessage = string() + MetadataCache::preferences.chatbotName + " | " + _sender.name + ", I cannot find the user you want to make a mod.\0";
			break;

		case SEARCH_USER_HISTORY_RESULT::ONLINE:
			handleGuest(GuestData(_onlineGuest.name, _onlineGuest.userID), true, _onlineGuest.id);
			_guestHistory.pop(_onlineGuest.userID);
			break;

		case SEARCH_USER_HISTORY_RESULT::OFFLINE:
			handleGuest(_offlineGuest, false);
			_guestHistory.pop(_offlineGuest.userID);
			break;

		case SEARCH_USER_HISTORY_RESULT::FAILED:
		default:
			_replyMessage = MetadataCache::preferences.chatbotName + " | Usage: !mod <username>\nExample: !mod melon\0";
			break;
		}

		return rv;
	}

	static vector<const char*> prefixes()
	{
		return vector<const char*> { "!mod", "!moderator" };
	}

private:
	static vector<const char*> internalPrefixes()
	{
		return vector<const char*> { "!mod ", "!moderator " };
	}

	ParsecDSO* _parsec;
	Guest& _sender;
	ModList& _mod;
	TierList& _tierList;

	bool handleGuest(GuestData target, bool isOnline, uint32_t guestID = -1)
	{
		bool result = false;

		if (_sender.userID == target.userID)
			_replyMessage = string() + MetadataCache::preferences.chatbotName + " | You don't need to be a mod...you're the boss, " + _sender.name + "!\0";
		else
		{
			_replyMessage = string() + MetadataCache::preferences.chatbotName + " | " + target.name + " was made a mod by " + _sender.name + "!\0";

			if (_mod.mod(target))
			{
				_tierList.setTier(target.userID, Tier::MOD);

				try {
					PlaySound(TEXT("./sfx/mod.wav"), NULL, SND_FILENAME | SND_NODEFAULT | SND_ASYNC);
				}
				catch (const std::exception&) {}

				result = true;
			}
		}

		return result;
	}
};