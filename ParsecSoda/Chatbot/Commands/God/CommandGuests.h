#pragma once

#include <sstream>
#include "parsec-dso.h"
#include "../Base/ACommandIntegerArg.h"

class CommandGuests : public ACommandIntegerArg
{
public:

	/**
	 * @brief Construct a new CommandGuests object
	 * 
	 * @param msg The message to parse
	 * @param config The host config
	 */
	CommandGuests(const char* msg, ParsecHostConfig &config)
		: ACommandIntegerArg(msg, internalPrefixes()), _config(config)
	{}

	/**
	 * @brief Run the command
	 * @return true if the command was successful
	 */
	bool run() override {
		if ( !ACommandIntegerArg::run() ) {
			SetReply("Usage: !guests <number>\nExample: !guests 7\0");
			return false;
		}

		_config.maxGuests = _intArg;
		SetReply("Max guests set to " + std::to_string(_intArg) + "\0");

		return true;
	}

	/**
	 * @brief Get the prefixes
	 * @return The prefixes
	 */
	static vector<const char*> prefixes() {
		return vector<const char*> { "!guests" };
	}

protected:
	static vector<const char*> internalPrefixes()
	{
		return vector<const char*> { "!guests " };
	}

	ParsecHostConfig& _config;
};
