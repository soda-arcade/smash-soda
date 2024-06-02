#pragma once

#include "../../../Core/Cache.h"
#include "../Base/ACommandSearchUserHistory.h"
#include "parsec-dso.h"

using namespace std;

class CommandBan : public ACommandSearchUserHistory
{
public:

	/**
	 * @brief Construct a new CommandBan object
	 * 
	 * @param msg 
	 * @param sender 
	 * @param parsec 
	 * @param guests 
	 * @param guestHistory 
	 */
	CommandBan(const char* msg, Guest& sender, ParsecDSO* parsec, GuestList &guests, GuestDataList &guestHistory)
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
			SetReply(_sender.name + ", I cannot find the user you want to ban.\0");
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
			SetReply("Usage: !ban <username>\nExample: !ban MK10\0");
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
		return vector<const char*> { "!ban", "!block" };
	}

private:
	static vector<const char*> internalPrefixes() {
		return vector<const char*> { "!ban ", "!block " };
	}

	ParsecDSO* _parsec;
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

		if (_sender.userID == target.userID) {
			SetReply("Thou shall not ban thyself, " + _sender.name + " ...\0");
		} 
		
		else if (Cache::cache.isSodaCop(target.userID)) {
			SetReply("Nice try bub, but you can't ban a Soda Cop!\0");
		} else {
			SetReply(
				Cache::cache.isSodaCop(_sender.userID) ?
				_sender.name + " laid down the law as a Soda Cop and banned " + target.name + "!\0"
				: _sender.name + " kicked " + target.name + "!\0"
			);

			if (Cache::cache.banList.ban(target)) {
				if (isOnline) {
					ParsecHostKickGuest(_parsec, guestID);
				}

				try {
					PlaySound(TEXT("./SFX/ban.wav"), NULL, SND_FILENAME | SND_NODEFAULT | SND_ASYNC);
				}
				catch (const std::exception&) {}

				result = true;
			}
		}

		return result;
	}
};