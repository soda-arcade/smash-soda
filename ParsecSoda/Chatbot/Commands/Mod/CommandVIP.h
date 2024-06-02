#pragma once

#include "../../../Core/Cache.h"
#include "../Base/ACommandSearchUserHistory.h"

using namespace std;

class CommandVIP : public ACommandSearchUserHistory
{
public:

	/**
	 * @brief Construct a new CommandVIP object
	 * 
	 * @param msg
	 * @param sender
	 * @param parsec
	 * @param guests
	 * @param guestHistory
	 */
	CommandVIP(const char* msg, Guest& sender, ParsecDSO* parsec, GuestList& guests, GuestDataList& guestHistory)
		: ACommandSearchUserHistory(msg, internalPrefixes(), guests, guestHistory), _sender(sender), _parsec(parsec)
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
			SetReply(_sender.name + ", I cannot find the user you want to make a VIP.\0");
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
			SetReply("Usage: !vip <username>\nExample: !vip melon\0");
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

	bool handleGuest(GuestData target, bool isOnline, uint32_t guestID = -1)
	{
		bool result = false;

		if (!Cache::cache.vipList.isVIP(target.userID)) {

			SetReply(target.name + " was made a VIP!");

			if (Cache::cache.vipList.VIP(target)) {

				try {
					PlaySound(TEXT("./SFX/vip.wav"), NULL, SND_FILENAME | SND_NODEFAULT | SND_ASYNC);
				}
				catch (const std::exception&) {}

				result = true;
			}

		}
		else {

			GuestData unmoddedGuest;
			bool found = false;

			try {
				found = Cache::cache.vipList.unVIP(target.name, [&unmoddedGuest](GuestData& guest) {
					unmoddedGuest = guest;
					});
			}
			catch (const std::exception&) { found = false; }

			if (found) {
				SetReply("Revoked VIP permissions for " + unmoddedGuest.name + "\0");
				_guestHistory.add(unmoddedGuest);
				return true;
			}

		}

		return result;
	}
};