#pragma once

#include "ACommandSearchUser.h"
#include <Windows.h>
#include <mmsystem.h>
#include <iostream>
#include <sstream>
#include "parsec.h"
#include "../Modules/AutoMod.h"

class CommandMute : public ACommandSearchUser
{
public:
	const COMMAND_TYPE type() override { return COMMAND_TYPE::BOT_MESSAGE; }

	CommandMute(const char* msg, Guest& sender, GuestList& guests, Guest& host)
		: ACommandSearchUser(msg, internalPrefixes(), guests), _sender(sender), _host(host)
	{}

	bool run() override
	{

		ACommandSearchUser::run();

		if (_searchResult != SEARCH_USER_RESULT::FOUND)
		{
			try
			{
				if (_host.userID == stoul(_targetUsername))
				{
					_targetGuest = _host;
					_searchResult = SEARCH_USER_RESULT::FOUND;
				}
			}
			catch (const std::exception&) {}

			if (_searchResult != SEARCH_USER_RESULT::FOUND && _targetUsername.compare(_host.name) == 0)
			{
				_targetGuest = _host;
				_searchResult = SEARCH_USER_RESULT::FOUND;
			}
		}


		bool rv = false;

		switch (_searchResult)
		{
		case SEARCH_USER_RESULT::NOT_FOUND:
			_replyMessage = std::string() + Config::cfg.chatbotName + _targetGuest.name + " shut up and left.\0";

		case SEARCH_USER_RESULT::FOUND:
			rv = true;
			if (_sender.userID == _targetGuest.userID) {
				_replyMessage = std::string() + Config::cfg.chatbotName + _sender.name + " tried to mute...themselves?\0";
			} else {
				AutoMod::instance.MuteUser(_targetGuest.userID, _targetGuest.name, 5);
			}
			break;

		case SEARCH_USER_RESULT::FAILED:
		default:
			_replyMessage = "[ChatBot] | Usage: !mute <username>\nExample: !mute bigboi83\0";
			break;
		}
		return rv;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	static vector<const char*> prefixes() {
		return vector<const char*> { "!mute" };
	}

protected:
	static vector<const char*> internalPrefixes() {
		return vector<const char*> { "!mute " };
	}

	Guest& _sender;
	Guest& _host;
};