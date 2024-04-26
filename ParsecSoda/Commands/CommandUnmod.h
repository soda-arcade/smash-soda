#pragma once

#include "ACommandStringArg.h"
#include <iostream>
#include <sstream>
#include "parsec-dso.h"
#include "../ModList.h"
#include "../Guest.h"

class CommandUnmod : public ACommandStringArg
{
public:
	const COMMAND_TYPE type() override { return COMMAND_TYPE::MOD; }

	CommandUnmod(const char* msg, Guest& sender, ModList& modList, GuestDataList& guestHistory, TierList& tierList)
		: ACommandStringArg(msg, internalPrefixes()), _sender(sender), _mod(modList), _guestHistory(guestHistory), _tierList(tierList)
	{}

	bool run() override
	{
		if (!ACommandStringArg::run())
		{
			_replyMessage = Config::cfg.chatbotName + "Usage: !unmod <username>\nExample: !unmod MickeyUK\0";
			return false;
		}

		GuestData unmoddedGuest;
		bool found = false;

		try
		{
			found = _mod.unmod(stoul(_stringArg), [&unmoddedGuest](GuestData& guest) {
				unmoddedGuest = guest;
				});
		}
		catch (const std::exception&) { found = false; }

		if (!found)
		{
			found = _mod.unmod(_stringArg, [&unmoddedGuest](GuestData& guest) {
				unmoddedGuest = guest;
				});
		}

		if (found)
		{
			std::ostringstream reply;
			reply
				<< Config::cfg.chatbotName << _sender.name << " has revoked mod permissions:\n"
				<< "\t\t" << unmoddedGuest.name << "\t(#" << unmoddedGuest.userID << ")\0";
			_replyMessage = reply.str();
			_guestHistory.add(unmoddedGuest);
			_tierList.setTier(unmoddedGuest.userID, Tier::PLEB);
			return true;
		}
		else
		{
			_replyMessage = std::string() + Config::cfg.chatbotName + _sender.name + ", I cannot find the user you want to unmod.\0";
			return false;
		}
	}

	static vector<const char*> prefixes()
	{
		return vector<const char*> { "!unmod" };
	}

protected:
	static vector<const char*> internalPrefixes()
	{
		return vector<const char*> { "!unmod " };
	}
	string _msg;
	Guest& _sender;
	ModList& _mod;
	GuestDataList& _guestHistory;
	TierList& _tierList;
};

