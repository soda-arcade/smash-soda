#pragma once

#include "ACommandSearchUserIntArg.h"
#include <Windows.h>
#include <mmsystem.h>
#include <iostream>
#include <sstream>
#include "parsec.h"
#include "../Modules/Hotseat.h"

class CommandExtendCD : public ACommandSearchUserIntArg
{
public:
	ParsecMetrics metrics;
	stringstream ping;

	const COMMAND_TYPE type() override { return COMMAND_TYPE::BOT_MESSAGE; }

	CommandExtendCD(const char* msg, Guest& sender, GuestList& guests, Guest& host)
		: ACommandSearchUserIntArg(msg, internalPrefixes(), guests), _sender(sender), _host(host)
	{}

	bool run() override {

		// Look for user
		ACommandSearchUserIntArg::run();
		if (_searchResult != SEARCH_USER_RESULT::FOUND) {
			try {
				if (_host.userID == stoul(targetUsername)) {
					_targetGuest = _host;
					_searchResult = SEARCH_USER_RESULT::FOUND;
				}
			}
			catch (const std::exception&) {}

			if (_searchResult != SEARCH_USER_RESULT::FOUND && targetUsername.compare(_host.name) == 0) {
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

			Hotseat::instance.screwUser(_targetGuest.userID, _intArg);

			break;

		case SEARCH_USER_RESULT::FAILED:
		default:
			_replyMessage = Config::cfg.chatbotName + "Usage: !screw <username> <n?\nExample: !screw bigboi83 6\0";
			break;
		}

		return rv;
	}

	static vector<const char*> prefixes() {
		return vector<const char*> { "!screw", "!extendcd", "!increasecd" };
	}

protected:
	static vector<const char*> internalPrefixes()
	{
		return vector<const char*> { "!screw " };
	}

	Guest& _sender;
	Guest& _host;
};