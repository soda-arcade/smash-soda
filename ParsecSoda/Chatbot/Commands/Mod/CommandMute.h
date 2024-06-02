#pragma once

#include "../Base/ACommandSearchUser.h"
#include "../../../Modules/AutoMod.h"

class CommandMute : public ACommandSearchUser
{
public:

	/**
	 * @brief Construct a new CommandMute object
	 * 
	 * @param msg
	 * @param sender
	 * @param guests
	 * @param host
	 */
	CommandMute(const char* msg, Guest& sender, GuestList& guests, Guest& host)
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
			SetReply(_targetGuest.name + " shut up and left.");

		case SEARCH_USER_RESULT::FOUND:
			rv = true;
			if (_sender.userID == _targetGuest.userID) {
				SetReply(_sender.name + " tried to mute...themselves?");
			} else {
				AutoMod::instance.MuteUser(_targetGuest.userID, _targetGuest.name, 5);
			}
			break;

		case SEARCH_USER_RESULT::FAILED:
		default:
			SetReply("Usage: !mute <username>\nExample: !mute bigboi83\0");
			break;
		}
		return rv;
	}

	/**
	 * @brief Get the prefixes object
	 * 
	 * @return std::vector<const char*>
	 */
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