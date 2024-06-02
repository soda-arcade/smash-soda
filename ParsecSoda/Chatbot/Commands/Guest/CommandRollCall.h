#pragma once

#include "../../../MetadataCache.h"
#include "../../../Core/Config.h"
#include "../Base/ACommand.h"
#include "../../../GuestList.h"

using namespace std;

class CommandRollCall : public ACommand {
public:

	/**
	 * @brief Construct a new CommandRollCall object
	 * 
	 * @param msg 
	 * @param sender 
	 * @param guestList 
	 */
	CommandRollCall(const char* msg, Guest& sender, GuestList& guestList)
		: _sender(sender), _guestList(guestList) {}

	/**
	 * @brief Run the command
	 * @return true if the command was successful
	 */
	bool run() override {

		string reponse = "[ACTIVE PLAYERS]\n";

		// For each guest in the guest list, send a message to them.
		for (auto& guest : _guestList.getGuests()) {

			if (MetadataCache::isSpectating(guest.userID)) {
				continue;
			}

			reponse += "[" + guest.name + "]\n";
		}

		_replyMessage = reponse;

		return true;

	}

	/**
	 * @brief Get the prefixes object
	 * @return vector<const char*> 
	 */
	static vector<const char*> prefixes() {
		return vector<const char*> { "!rollcall" };
	}

protected:

	Guest& _sender;
	GuestList& _guestList;

	static vector<const char*> internalPrefixes() {
		return vector<const char*> { "!rollcall " };
	}
};