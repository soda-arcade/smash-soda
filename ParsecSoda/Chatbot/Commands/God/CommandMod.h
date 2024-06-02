#pragma once

#include "../../../Core/Cache.h"
#include "../Base/ACommandSearchUserHistory.h"
#include <iostream>
#include <Windows.h>
#include <mmsystem.h>
#include "parsec-dso.h"

using namespace std;

class CommandMod : public ACommandSearchUserHistory
{
public:

	/**
	 * @brief Construct a new CommandMod object
	 * 
	 * @param msg The message to parse
	 * @param sender The sender of the message
	 * @param parsec The parsec instance
	 * @param guests The list of online guests
	 * @param guestHistory The list of offline guests
	 */
	CommandMod(const char* msg, Guest& sender, ParsecDSO* parsec, GuestList& guests, GuestDataList& guestHistory)
		: ACommandSearchUserHistory(msg, internalPrefixes(), guests, guestHistory), _sender(sender), _parsec(parsec)
	{
	}

	/**
	 * @brief Run the command
	 * @return true if the command was successful
	 */
	bool run() override {
		ACommandSearchUserHistory::run();

		bool rv = false;

		switch (_searchResult)
		{
		case SEARCH_USER_HISTORY_RESULT::NOT_FOUND:
			SetReply(_sender.name + ", I cannot find the user you want to make a mod.\0");
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
			SetReply("Usage: !mod <username>\nExample: !mod Call_Me_Troy\0");
			break;
		}

		return rv;
	}

	/**
	 * @brief Get the prefixes
	 * @return The prefixes
	 */
	static vector<const char*> prefixes() {
		return vector<const char*> { "!mod", "!moderator" };
	}

private:
	static vector<const char*> internalPrefixes()
	{
		return vector<const char*> { "!mod ", "!moderator " };
	}

	ParsecDSO* _parsec;
	Guest& _sender;

	/**
	 * @brief Handle the guest
	 * 
	 * @param target The guest to handle
	 * @param isOnline True if the guest is online
	 * @param guestID The guest ID
	 * @return true if the guest was handled
	 */
	bool handleGuest(GuestData target, bool isOnline, uint32_t guestID = -1) {
		bool result = false;

		if (_sender.userID == target.userID) {
			SetReply("You don't need to be a mod...you're head honcho, " + _sender.name + "!\0");
		} else {
			SetReply(_sender.name + " has made " + target.name + " a mod!\0");

			if (Cache::cache.modList.mod(target)) {
				
				Cache::cache.tierList.setTier(target.userID, Tier::MOD);

				try {
					PlaySound(TEXT("./SFX/mod.wav"), NULL, SND_FILENAME | SND_NODEFAULT | SND_ASYNC);
				}
				catch (const std::exception&) {}

				result = true;
			}
		}

		return result;
	}
};