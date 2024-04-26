#pragma once

#include "ACommandStringArg.h"
#include <iostream>
#include <sstream>
#include "parsec-dso.h"
#include "../VIPList.h"
#include "../Guest.h"

class CommandUnVIP : public ACommandStringArg
{
public:
	const COMMAND_TYPE type() override { return COMMAND_TYPE::MOD; }

	CommandUnVIP(const char* msg, Guest& sender, VIPList& vipList, GuestDataList& guestHistory, TierList& tierList)
		: ACommandStringArg(msg, internalPrefixes()), _sender(sender), _vip(vipList), _guestHistory(guestHistory), _tierList(tierList)
	{}

	bool run() override
	{
		if (!ACommandStringArg::run())
		{
			_replyMessage = Config::cfg.chatbotName + "Usage: !unvip <username>\nExample: !unvip MickeyUK\0";
			return false;
		}

		GuestData unmoddedGuest;
		bool found = false;

		try
		{
			found = _vip.unVIP(stoul(_stringArg), [&unmoddedGuest](GuestData& guest) {
				unmoddedGuest = guest;
				});
		}
		catch (const std::exception&) { found = false; }

		if (!found)
		{
			found = _vip.unVIP(_stringArg, [&unmoddedGuest](GuestData& guest) {
				unmoddedGuest = guest;
				});
		}

		if (found)
		{
			std::ostringstream reply;
			reply
				<< Config::cfg.chatbotName << _sender.name << " has revoked VIP permissions for: \n"
				<< "\t\t" << unmoddedGuest.name << "\t(#" << unmoddedGuest.userID << ")\0";
			_replyMessage = reply.str();
			_guestHistory.add(unmoddedGuest);
			return true;
		}
		else
		{
			_replyMessage = std::string() + Config::cfg.chatbotName + _sender.name + ", I cannot find the user you want to unvip.\0";
			return false;
		}
	}

	static vector<const char*> prefixes()
	{
		return vector<const char*> { "!unvip" };
	}

protected:
	static vector<const char*> internalPrefixes()
	{
		return vector<const char*> { "!unvip " };
	}
	string _msg;
	Guest& _sender;
	VIPList& _vip;
	GuestDataList& _guestHistory;
	TierList& _tierList;
};

