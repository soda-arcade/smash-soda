#pragma once

#include "parsec-dso.h"
#include "ACommandStringArg.h"
#include <algorithm>
#include <random>

class Command2v2 : public ACommandStringArg {
public:
	const COMMAND_TYPE type() override { return COMMAND_TYPE::BOT_MESSAGE; }

	Command2v2(const char* msg, GuestList& guests, GamepadClient& gamepadClient)
		: ACommandStringArg(msg, internalPrefixes()), _guests(guests), _gamepadClient(gamepadClient)
	{}

	/// <summary>
	/// When command is first executed.
	/// </summary>
	/// <returns>Boolean</returns>
	bool run() override {

		// No sub command given
		if (!ACommandStringArg::run()) {

			// Start tournament
			if (!MetadataCache::tournament.isInit) {
				//_replyMessage = "[TourneyBot] " + Tournament::init(Tournament::Type::BRACKETS, 2);
				return true;
			}
			else {
				_replyMessage = "[TourneyBot] | Usage: https://github.com/MickeyUK/SmashSoda/wiki/Tournaments\0";
				return false;
			}
			
		}

		// Sub commands
		//if (_stringArg == "start") _replyMessage = "[TourneyBot] " + Tournament::start();
		//if (_stringArg == "stop") _replyMessage = "[TourneyBot] " + Tournament::stop();
		//if (_stringArg == "shuffle") _replyMessage = "[TourneyBot] " + Tournament::shuffle();
		//if (_stringArg == "brackets") _replyMessage = "[TourneyBot] " + Tournament::brackets();

		return true;

	}

	/// <summary>
	/// Command prefixes.
	/// </summary>
	/// <returns>Vector</returns>
	static vector<const char*> prefixes() {
		return vector<const char*> { "!2v2" };
	}

protected:

	GuestList& _guests;
	GamepadClient& _gamepadClient;

	/// <summary>
	/// Internal prefixes.
	/// </summary>
	/// <returns>Vector</returns>
	static vector<const char*> internalPrefixes() {
		return vector<const char*> { "!2v2 " };
	}

};