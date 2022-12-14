#pragma once

#include "ACommandSearchUserHistory.h"
#include <iostream>
#include <Windows.h>
#include <mmsystem.h>
#include "parsec-dso.h"
#include "../VipList.h"

using namespace std;

class CommandVIP : public ACommandSearchUserHistory
{
public:
	const COMMAND_TYPE type() override { return COMMAND_TYPE::MOD; }

	CommandVIP(const char* msg, Guest& sender, ParsecDSO* parsec, GuestList& guests, GuestDataList& guestHistory, VIPList& vipList, TierList& tierList)
		: ACommandSearchUserHistory(msg, internalPrefixes(), guests, guestHistory), _sender(sender), _parsec(parsec), _vip(vipList), _tierList(tierList)
	{
	}

	bool run() override
	{
		ACommandSearchUserHistory::run();

		bool rv = false;

		switch (_searchResult)
		{
		case SEARCH_USER_HISTORY_RESULT::NOT_FOUND:
			_replyMessage = string() + MetadataCache::preferences.chatbotName + " | " + _sender.name + ", I cannot find the user you want to make a VIP.\0";
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
			_replyMessage = MetadataCache::preferences.chatbotName + " | Usage: !vip <username>\nExample: !vip melon\0";
			break;
		}

		return rv;
	}

	static vector<const char*> prefixes()
	{
		return vector<const char*> { "!vip" };
	}

private:
	static vector<const char*> internalPrefixes()
	{
		return vector<const char*> { "!vip " };
	}

	ParsecDSO* _parsec;
	Guest& _sender;
	VIPList& _vip;
	TierList& _tierList;

	bool handleGuest(GuestData target, bool isOnline, uint32_t guestID = -1)
	{
		bool result = false;

		_replyMessage = string() + MetadataCache::preferences.chatbotName + " | " + target.name + " was made a VIP!\0";

		if (_vip.VIP(target))
		{

			try {
				PlaySound(TEXT("./SFX/vip.wav"), NULL, SND_FILENAME | SND_NODEFAULT | SND_ASYNC);
			}
			catch (const std::exception&) {}

			result = true;
		}

		return result;
	}
};