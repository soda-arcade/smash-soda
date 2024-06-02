#pragma once

#include "../../../Core/Cache.h"
#include "../Base/ACommandStringArg.h"
#include <iostream>
#include <sstream>
#include "parsec-dso.h"
#include "../../../Guest.h"

class CommandUnmod : public ACommandStringArg
{
public:

	/**
	 * @brief Construct a new CommandUnmod object
	 * 
	 * @param msg The message to parse
	 * @param sender The sender of the message
	 * @param guestHistory The list of offline guests
	 */
	CommandUnmod(const char* msg, Guest& sender, GuestDataList& guestHistory)
		: ACommandStringArg(msg, internalPrefixes()), _sender(sender), _guestHistory(guestHistory)
	{}

	/**
	 * @brief Run the command
	 * @return true if the command was successful
	 */
	bool run() override {

		// Check if the command was successful
		if (!ACommandStringArg::run()) {
			SetReply("Usage: !unmod <username>\nExample: !unmod MickeyUK\0");
			return false;
		}

		GuestData unmoddedGuest;
		bool found = false;

		try
		{
			found = Cache::cache.modList.unmod(stoul(_stringArg), [&unmoddedGuest](GuestData& guest) {
				unmoddedGuest = guest;
				});
		}
		catch (const std::exception&) { found = false; }

		if (!found)
		{
			found = Cache::cache.modList.unmod(_stringArg, [&unmoddedGuest](GuestData& guest) {
				unmoddedGuest = guest;
				});
		}

		if (found)
		{
			SetReply("Unmodded " + unmoddedGuest.name + "\0");
			_guestHistory.add(unmoddedGuest);
			Cache::cache.tierList.setTier(unmoddedGuest.userID, Tier::PLEB);
			return true;
		}
		else
		{
			SetReply(_sender.name + ", I cannot find the user you want to unmod.\0");
			return false;
		}
	}

	/**
	 * @brief Get the prefixes
	 * @return The prefixes
	 */
	static vector<const char*> prefixes() {
		return vector<const char*> { "!unmod" };
	}

protected:
	static vector<const char*> internalPrefixes()
	{
		return vector<const char*> { "!unmod " };
	}
	string _msg;
	Guest& _sender;
	GuestDataList& _guestHistory;
};

