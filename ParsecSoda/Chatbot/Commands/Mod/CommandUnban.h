#pragma once

#include "../Base/ACommandStringArg.h"
#include "../../../Guest.h"

class CommandUnban : public ACommandStringArg
{
public:

	/**
	 * @brief Construct a new CommandUnban object
	 * 
	 * @param msg
	 * @param sender
	 * @param guestHistory
	 */
	CommandUnban(const char* msg, Guest &sender, GuestDataList& guestHistory)
		: ACommandStringArg(msg, internalPrefixes()), _sender(sender), _guestHistory(guestHistory)
	{}

	/**
	 * @brief Run the command
	 * 
	 * @return true
	 * @return false
	 */
	bool run() override {
		if ( !ACommandStringArg::run()) {
			SetReply("Usage: !unban <username>\nExample: !unban DIO\0");
			return false;
		}
		
		GuestData unbannedGuest;
		bool found = false;

		try {
			found = Cache::cache.banList.unban(stoul(_stringArg), [&unbannedGuest](GuestData& guest) {
				unbannedGuest = guest;
			});
		}
		catch (const std::exception&) { found = false; }

		if (!found) {
			found = Cache::cache.banList.unban(_stringArg, [&unbannedGuest](GuestData& guest) {
				unbannedGuest = guest;
			});
		}

		if (found) {
			std::ostringstream reply;
			SetReply("Unbanned " + unbannedGuest.name + "\0");
			_guestHistory.add(unbannedGuest);
			return true;
		}
		else {
			SetReply(_sender.name + ", I cannot find the user you want to unban.\0");
			return false;
		}
	}

	/**
	 * @brief Get the prefixes object
	 * 
	 * @return vector<const char*>
	 */
	static vector<const char*> prefixes() {
		return vector<const char*> { "!unban" };
	}

protected:
	static vector<const char*> internalPrefixes()
	{
		return vector<const char*> { "!unban " };
	}
	string _msg;
	Guest& _sender;
	GuestDataList& _guestHistory;
};

