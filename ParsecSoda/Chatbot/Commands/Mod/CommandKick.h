#pragma once

#include "../Base/ACommandSearchUser.h"
#include "parsec-dso.h"


class CommandKick : public ACommandSearchUser
{
public:

	/**
	 * @brief Construct a new CommandKick object
	 * 
	 * @param msg
	 * @param sender
	 * @param parsec
	 * @param guests
	 * @param isHost
	 */
	CommandKick(const char* msg, Guest& sender, ParsecDSO* parsec, GuestList &guests, bool isHost = false)
		:ACommandSearchUser(msg, internalPrefixes(), guests), _sender(sender), _parsec(parsec), _isHost(isHost)
	{}

	/**
	 * @brief Run the command
	 * 
	 * @return true
	 * @return false
	 */
	bool run() override {
		ACommandSearchUser::run();
		
		switch (_searchResult)
		{
		case SEARCH_USER_RESULT::NOT_FOUND:
			{
				SetReply(_sender.name + ", I cannot find the user you want to kick.");
			}
			break;

		case SEARCH_USER_RESULT::FOUND:

			if (_sender.userID == _targetGuest.userID) {
				SetReply(_sender.name + " kicked themselves in confusion!");
				ParsecHostKickGuest(_parsec, _targetGuest.id);
			}
			
			else if (Cache::cache.isSodaCop(_targetGuest.userID)) {
				SetReply("Nice try bub, but you can't kick a Soda Cop!\0");
			} else {
				SetReply(
					Cache::cache.isSodaCop(_sender.userID) ?
					_sender.name + " laid down the law as a Soda Cop and kicked " + _targetGuest.name + "!\0"
					: _sender.name + " kicked " + _targetGuest.name + "!\0"
				);
				ParsecHostKickGuest(_parsec, _targetGuest.id);
				
				//try
				//{
				//	PlaySound(TEXT("./SFX/kick.wav"), NULL, SND_FILENAME | SND_NODEFAULT | SND_ASYNC);
				//}
				//catch (const std::exception&) {}

				return true;
			}
			break;
		
		case SEARCH_USER_RESULT::FAILED:
		default:
			SetReply("Usage: !kick <username>\nExample: !kick MickeyUK");
			break;
		}

		return false;
	}

	/**
	 * @brief Get the prefixes object
	 * 
	 * @return vector<const char*>
	 */
	static vector<const char*> prefixes() {
		return vector<const char*> { "!kick" };
	}

protected:
	static vector<const char*> internalPrefixes() {
		return vector<const char*> { "!kick " };
	}

	Guest& _sender;
	ParsecDSO* _parsec;
	bool _isHost;
};

