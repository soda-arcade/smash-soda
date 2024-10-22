#pragma once

#include "../Base/ACommandSearchUserIntArg.h"
#include "../../../Modules/Hotseat.h"

class CommandDecrease : public ACommandSearchUserIntArg
{
public:

	/**
	 * @brief Construct a new CommandDecrease object
	 * 
	 * @param msg
	 * @param sender
	 * @param guests
	 * @param host
	 */
	CommandDecrease(const char* msg, Guest& sender, GuestList& guests, Guest& host)
		: ACommandSearchUserIntArg(msg, internalPrefixes(), guests), _sender(sender), _host(host)
	{}

	/**
	 * @brief Run the command
	 * 
	 * @return true
	 * @return false
	 */
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
			Hotseat::instance.deductUser(_targetGuest.userID, _intArg);
			break;

		case SEARCH_USER_RESULT::FAILED:
		default:
			SetReply("Usage: !decrease <username> <n>\nExample: !decrease RefffiK 6\0");
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
		return vector<const char*> { "!decrease", "!taketime", "!subtract" };
	}

protected:
	static vector<const char*> internalPrefixes()
	{
		return vector<const char*> { "!decrease " };
	}

	Guest& _sender;
	Guest& _host;
};