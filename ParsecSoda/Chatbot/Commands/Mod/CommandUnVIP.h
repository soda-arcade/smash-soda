#pragma once

#include "../../../Core/Cache.h"
#include "../Base/ACommandStringArg.h"
#include "../../../Guest.h"

class CommandUnVIP : public ACommandStringArg
{
public:

	/**
	 * @brief Construct a new CommandUnVIP object
	 * 
	 * @param msg
	 * @param sender
	 * @param guestHistory
	 */
	CommandUnVIP(const char* msg, Guest& sender, GuestDataList& guestHistory)
		: ACommandStringArg(msg, internalPrefixes()), _sender(sender), _guestHistory(guestHistory)
	{}

	/**
	 * @brief Run the command
	 * 
	 * @return true
	 * @return false
	 */
	bool run() override {
		if (!ACommandStringArg::run()) {
			SetReply("Usage: !unvip <username>\nExample: !unvip MickeyUK\0");
			return false;
		}

		GuestData unmoddedGuest;
		bool found = false;

		try {
			found = Cache::cache.vipList.unVIP(stoul(_stringArg), [&unmoddedGuest](GuestData& guest) {
				unmoddedGuest = guest;
			});
		}
		catch (const std::exception&) { found = false; }

		if (!found)
		{
			found = Cache::cache.vipList.unVIP(_stringArg, [&unmoddedGuest](GuestData& guest) {
				unmoddedGuest = guest;
				});
		}

		if (found)
		{
			SetReply("UnVIP'd " + unmoddedGuest.name + "\0");
			_guestHistory.add(unmoddedGuest);
			return true;
		}
		else
		{
			SetReply(_sender.name + ", I cannot find the user you want to unvip.\0");
			return false;
		}
	}

	/**
	 * @brief Get the prefixes object
	 * 
	 * @return vector<const char*>
	 */
	static vector<const char*> prefixes() {
		return vector<const char*> { "!unvip" };
	}

protected:
	static vector<const char*> internalPrefixes()
	{
		return vector<const char*> { "!unvip " };
	}
	string _msg;
	Guest& _sender;
	GuestDataList& _guestHistory;
};

