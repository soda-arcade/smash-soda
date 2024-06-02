#pragma once

#include "../../../Core/Config.h"
#include "../Base/ACommandSearchUser.h"
#include "../../../Modules/AutoMod.h"

class CommandUnmute : public ACommandSearchUser
{
public:

	/**
	 * @brief Construct a new CommandUnmute object
	 * 
	 * @param msg
	 * @param sender
	 * @param guests
	 * @param host
	 */
	CommandUnmute(const char* msg, Guest& sender, GuestList& guests, Guest& host)
		: ACommandSearchUser(msg, internalPrefixes(), guests), _sender(sender), _host(host)
	{}

	/**
	 * @brief Run the command
	 * 
	 * @return true
	 * @return false
	 */
	bool run() override {

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
			SetReply(_targetUsername + " is not in the chat.\0");

		case SEARCH_USER_RESULT::FOUND:
			rv = true;
			if (_sender.userID == _targetGuest.userID) {
				SetReply(_sender.name + " tried to mute...themselves?\0");
			}
			else {
				AutoMod::instance.UnmuteUser(_targetGuest.userID);
			}
			break;

		case SEARCH_USER_RESULT::FAILED:
		default:
			_replyMessage = "Usage: !unmute <username>\nExample: !unmute bigboi83\0";
			break;
		}

		return rv;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	static vector<const char*> prefixes() {
		return vector<const char*> { "!unmute" };
	}

protected:
	static vector<const char*> internalPrefixes() {
		return vector<const char*> { "!unmute " };
	}

	Guest& _sender;
	Guest& _host;
};