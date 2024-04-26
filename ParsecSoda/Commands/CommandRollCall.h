#pragma once

#include "../MetadataCache.h"
#include "../Core/Config.h"
#include "ACommand.h"
#include "../GuestList.h"

using namespace std;

/// <summary>
/// This command prints a list of all the 
/// players who are not spectating.
/// </summary>
class CommandRollCall : public ACommand {
public:

	// Leave this unchanged unless you know what you're doing.
	const COMMAND_TYPE type() override { return COMMAND_TYPE::BOT_MESSAGE; }

	CommandRollCall(const char* msg, Guest& sender, GuestList& guestList)
		: _sender(sender), _guestList(guestList) {}

	/// <summary>
	/// This is the main function that will be called when the command is run.
	/// </summary>
	/// <returns></returns>
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

	/// <summary>
	/// These are the prefixes that will trigger the command when a user
	/// types them in chat.
	/// </summary>
	/// <returns></returns>
	static vector<const char*> prefixes() {
		return vector<const char*> { "!rollcall" };
	}

protected:

	Guest& _sender;
	GuestList& _guestList;

	/// <summary>
	/// Used to set the prefixes for the command when 
	/// using a string or integer argument.
	/// </summary>
	/// <returns></returns>
	static vector<const char*> internalPrefixes() {
		return vector<const char*> { "!rollcall " };
	}
};