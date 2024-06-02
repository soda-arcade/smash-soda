#pragma once

#include "../../../Core/Cache.h"
#include "../../../Core/Config.h"
#include "../Base/ACommandSearchUserHistory.h"

using namespace std;

class CommandVerify : public ACommandSearchUserHistory
{
public:

	/**
	 * @brief Construct a new CommandVerify object
	 * 
	 * @param msg
	 * @param sender
	 * @param guests
	 * @param guestHistory
	 */
	CommandVerify(const char* msg, Guest& sender, GuestList& guests, GuestDataList& guestHistory)
		: ACommandSearchUserHistory(msg, internalPrefixes(), guests, guestHistory), _sender(sender)
	{
	}

	/**
	 * @brief Run the command
	 * 
	 * @return true
	 * @return false
	 */
	bool run() override {
		ACommandSearchUserHistory::run();

		bool rv = false;

		switch (_searchResult)
		{
		case SEARCH_USER_HISTORY_RESULT::NOT_FOUND:
			SetReply(_targetUsername + " is not in the chat.\0");
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
			SetReply("Usage: !verify <User ID>\nExample: !verify 1693946\0");
			break;
		}

		return rv;
	}

	/**
	 * @brief Get the prefixes object
	 * 
	 * @return vector<const char*>
	 */
	static vector<const char*> prefixes() {
		return vector<const char*> { "!verify" };
	}

private:

	/**
	 * @brief Get the internal prefixes object
	 * 
	 * @return vector<const char*>
	 */
	static vector<const char*> internalPrefixes() {
		return vector<const char*> { "!verify " };
	}

	Guest& _sender;

	/**
	 * @brief Handle the guest
	 * 
	 * @param target
	 * @param isOnline
	 * @param guestID
	 * @return true
	 * @return false
	 */
	bool handleGuest(GuestData target, bool isOnline, uint32_t guestID = -1) {
		bool result = false;

		if (!Cache::cache.verifiedList.inList(target.userID)) {

			if (Cache::cache.verifiedList.Add(target)) {
				SetReply("Verified " + target.name + "\0");
			} else {
				SetReply("Failed to verify " + target.name + ".\0");
			}

		}
		else {

			GuestData verifiedGuest;
			bool found = false;

			try {
				found = Cache::cache.verifiedList.Remove(target.userID, [&verifiedGuest](GuestData& guest) {
					verifiedGuest = guest;
				});
			}
			catch (const std::exception&) { found = false; }

			if (found) {
				SetReply("Unverified " + verifiedGuest.name + "\0");
				_guestHistory.add(verifiedGuest);
				return true;
			}

		}

		return result;
	}
};