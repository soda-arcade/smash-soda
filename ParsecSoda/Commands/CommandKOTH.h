#pragma once

#include <iostream>
#include <sstream>
#include "ACommandIntegerArg.h"
#include "../GuestList.h"
#include "../GamepadClient.h"

class CommandKOTH : public ACommandIntegerArg
{
public:
	const COMMAND_TYPE type() override { return COMMAND_TYPE::BOT_MESSAGE; }

	CommandKOTH(const char* msg, GuestList& guestList, GamepadClient& gamepadClient)
		: ACommandIntegerArg(msg, internalPrefixes()), _guestList(guestList), _gamepadClient(gamepadClient)
	{}

	bool run() override
	{
		if (!ACommandIntegerArg::run()) {

			// If not already started
			if (!MetadataCache::teams.isStarted)
				init();
			else
				_replyMessage = "[KOTHBot] A tournament has already been started. Use \"!koth stop\" to stop.\0";
			return true;
		}

		_replyMessage = "\0";
		return true;
	}

	/// <summary>
	/// Creates a new KOTH tournament.
	/// </summary>
	void init() {

		// Do we have enough players?
		int activeGuestsTotal = getActiveGuestsTotal();

	}

	/// <summary>
	/// Get a list of all the guests that can play.
	/// </summary>
	int getActiveGuestsTotal() {

		return 0;

	}

	static vector<const char*> prefixes() {
		return vector<const char*> { "!koth" };
	}

protected:
	static vector<const char*> internalPrefixes() {
		return vector<const char*> { "!koth " };
	}
	string _msg;
	GuestList& _guestList;
	GamepadClient& _gamepadClient;
};