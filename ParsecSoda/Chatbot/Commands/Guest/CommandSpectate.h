#pragma once

#include "../../../Core/Cache.h"
#include "../Base/ACommandSearchUser.h"
#include "../../../Modules/Hotseat.h"

using namespace std;

class CommandSpectate : public ACommandSearchUser
{
public:

	/**
	 * @brief Construct a new CommandSpectate object
	 * 
	 * @param msg 
	 * @param sender 
	 * @param guests 
	 * @param gamepadClient 
	 * @param hotseat 
	 */
	CommandSpectate(const char* msg, Guest& sender, GuestList& guests, GamepadClient& gamepadClient, Hotseat& hotseat)
		: ACommandSearchUser(msg, internalPrefixes(), guests), _sender(sender), _gamepadClient(gamepadClient), _hotseat(hotseat)
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
			if (Cache::cache.tierList.getTier(_sender.userID) != Tier::PLEB) {
				_replyMessage = std::string() + Config::cfg.chatbotName + _sender.name + ", I cannot find the user you want to set to spectator.\0";
			}
			break;

		case SEARCH_USER_RESULT::FOUND:

			if (Cache::cache.tierList.getTier(_sender.userID) != Tier::PLEB) {
				addGuestToSpectators(_targetGuest);
			}
			
			break;
		
		case SEARCH_USER_RESULT::FAILED:
		default:
			
			addGuestToSpectators(_sender);

			break;
		}

		return true;
	}

	/**
	 * @brief Add a guest to the spectators list
	 * 
	 * @param guest 
	 * @return true 
	 * @return false 
	 */
	bool addGuestToSpectators(Guest guest) {

		GuestData guestData = GuestData(guest.name, guest.userID);

		if (MetadataCache::isSpectating(guest.userID)) {

			MetadataCache::addActiveGuest(guest);
			GuestData guestData = GuestData(guest.name, guest.userID);
			_replyMessage = Config::cfg.chatbotName + guest.name + " is no longer spectating.\0";
			
		}
		else {

			MetadataCache::removeActiveGuest(guest);
			_replyMessage = Config::cfg.chatbotName + guest.name + " is now spectating.\0";
			_gamepadClient.onQuit(guest);

		}

		return true;

	}

	/**
	 * @brief Get the prefixes object
	 * 
	 * @return vector<const char*> 
	 */
	static vector<const char*> prefixes() {
		return vector<const char*> { "!spectate", "!afk", "!lurk" };
	}

protected:
	static vector<const char*> internalPrefixes()
	{
		return vector<const char*> { "!spectate " };
	}

	Guest& _sender;
	GamepadClient& _gamepadClient;
	Hotseat& _hotseat;
};