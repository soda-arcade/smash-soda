#pragma once

#include "ACommandSearchUser.h"
#include <Windows.h>
#include <mmsystem.h>
#include <iostream>
#include <sstream>
#include "parsec.h"
#include "../Modules/Hotseat.h"

class CommandDecrease : public ACommandSearchUser
{
public:
	ParsecMetrics metrics;
	stringstream ping;

	const COMMAND_TYPE type() override { return COMMAND_TYPE::BOT_MESSAGE; }

	CommandDecrease(const char* msg, Guest& sender, GuestList& guests, Guest& host)
		: ACommandSearchUser(msg, internalPrefixes(), guests), _sender(sender), _host(host)
	{}

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
			break;

		case SEARCH_USER_RESULT::FOUND:

			rv = true;
			Hotseat::instance.deductUser(_targetGuest.userID, 5);
			break;

		case SEARCH_USER_RESULT::FAILED:
		default:
			_replyMessage = Config::cfg.chatbotName + "Usage: !extend <username>\nExample: !decrease bigboi83\0";
			break;
		}

		return rv;
	}

	static vector<const char*> prefixes() {
		return vector<const char*> { "!decrease", "!deduct" };
	}

protected:
	static vector<const char*> internalPrefixes()
	{
		return vector<const char*> { "!decrease " };
	}

	Guest& _sender;
	Guest& _host;
};