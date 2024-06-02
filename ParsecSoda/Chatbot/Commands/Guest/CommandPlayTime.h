#pragma once

#include "../Base/ACommandSearchUser.h"
#include "parsec.h"
#include "../../../Modules/Hotseat.h"
#include "../../../Core/Config.h"

class CommandPlayTime : public ACommandSearchUser
{
public:
	ParsecMetrics metrics;
	stringstream ping;

	/**
	 * @brief Construct a new CommandPlayTime object
	 * 
	 * @param msg 
	 * @param sender 
	 * @param guests 
	 * @param host 
	 */
	CommandPlayTime(const char* msg, Guest& sender, GuestList& guests, Guest& host)
		: ACommandSearchUser(msg, internalPrefixes(), guests), _sender(sender), _host(host)
	{}

	/**
	 * @brief Run the command
	 * @return true if the command was successful
	 */
	bool run() override {

		// Look for user
		ACommandSearchUser::run();
		if (_searchResult != SEARCH_USER_RESULT::FOUND) {
			try {
				if (_host.userID == stoul(_targetUsername)) {
					_targetGuest = _host;
					_searchResult = SEARCH_USER_RESULT::FOUND;
				}
			}
			catch (const std::exception&) {}

			if (_searchResult != SEARCH_USER_RESULT::FOUND && _targetUsername.compare(_host.name) == 0) {
				_targetGuest = _host;
				_searchResult = SEARCH_USER_RESULT::FOUND;
			}
		}


		bool rv = false;

		switch (_searchResult) {
		case SEARCH_USER_RESULT::NOT_FOUND:

			SetReply(_sender.name + " just missed the guest they tried to ping check.\0");

			break;

		case SEARCH_USER_RESULT::FOUND:

			rv = true;
			metrics = _guests.getMetrics(_targetGuest.id).metrics;
			ping << trunc(metrics.networkLatency);

			SetReply(_targetGuest.name + " has " + Hotseat::instance.getUserTimeRemaining(_targetGuest.userID) 
			+ " of play time remaining. Their play time will refresh in " 
			+ Hotseat::instance.getCooldownRemaining(_targetGuest.userID) + ".\0");

			break;

		case SEARCH_USER_RESULT::FAILED:
		default:
			SetReply("Usage: !playtime <username>\nExample: !playtime bigboi83\0");
			break;
		}

		return rv;
	}

	/**
	 * @brief Get the prefixes
	 * @return vector<const char*> 
	 */
	static vector<const char*> prefixes() {
		return vector<const char*> { "!playtime" };
	}

protected:
	static vector<const char*> internalPrefixes() {
		return vector<const char*> { "!playtime " };
	}

	Guest& _sender;
	Guest& _host;
};