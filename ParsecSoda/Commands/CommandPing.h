#pragma once

#include "ACommandSearchUser.h"
#include <Windows.h>
#include <mmsystem.h>
#include <iostream>
#include <sstream>
#include "parsec.h"

class CommandPing : public ACommandSearchUser
{
public:
	ParsecMetrics metrics;
	stringstream ping;

	const COMMAND_TYPE type() override { return COMMAND_TYPE::BOT_MESSAGE; }

	CommandPing(const char* msg, Guest& sender, GuestList& guests, Guest& host)
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

			_replyMessage = std::string() + Config::cfg.chatbotName + " | " + _sender.name + " just missed the guest they tried to ping check.\0";

			break;

		case SEARCH_USER_RESULT::FOUND:

			rv = true;
			metrics = _guests.getMetrics(_targetGuest.id).metrics;
			ping << trunc(metrics.networkLatency);

			_replyMessage = std::string() + Config::cfg.chatbotName + _targetGuest.name + ": [" + ping.str() + "ms]\0";

			break;

		case SEARCH_USER_RESULT::FAILED:
		default:
			_replyMessage = Config::cfg.chatbotName + " | Usage: !ping <username>\nExample: !ping bigboi83\0";
			break;
		}

		return rv;
	}

	static vector<const char*> prefixes() {
		return vector<const char*> { "!ping", "!latency" };
	}

protected:
	static vector<const char*> internalPrefixes()
	{
		return vector<const char*> { "!ping " };
	}

	Guest& _sender;
	Guest& _host;
};