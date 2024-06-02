#pragma once

#include "parsec-dso.h"
#include "../Base/ACommandIntegerArg.h"

class CommandSpot : public ACommandIntegerArg
{
public:

	/**
	 * @brief Construct a new CommandSpot object
	 * 
	 * @param msg
	 * @param config
	 */
	CommandSpot(const char* msg, ParsecHostConfig& config)
		: ACommandIntegerArg(msg, internalPrefixes()), _config(config)
	{}

	/**
	 * @brief Run the command
	 * 
	 * @return true
	 * @return false
	 */
	bool run() override {
		if (!ACommandIntegerArg::run()) {
			SetReply("Usage: !spot <number>\nExample: !spot 1\0");
			return false;
		}

		_config.maxGuests += _intArg;
		Config::cfg.roomChanged = true;
		SetReply("Added " + to_string(_intArg) + " guest slot(s)\0");

		return true;
	}

	/**
	 * @brief Get the prefixes object
	 * 
	 * @return vector<const char*>
	 */
	static vector<const char*> prefixes() {
		return vector<const char*> { "!spot" };
	}

protected:
	static vector<const char*> internalPrefixes()
	{
		return vector<const char*> { "!spot " };
	}

	ParsecHostConfig& _config;
};