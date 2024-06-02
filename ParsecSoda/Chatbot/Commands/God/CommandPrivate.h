#pragma once

#include "parsec-dso.h"
#include "../Base/ACommand.h"
#include <iostream>

class CommandPrivate : public ACommand
{
public:

	/**
	 * @brief Construct a new CommandPrivate object
	 * 
	 * @param config The host config
	 */
	CommandPrivate(ParsecHostConfig& config)
		: _config(config)
	{}

	/**
	 * @brief Run the command
	 * @return true if the command was successful
	 */
	bool run() override {
		_replyMessage = Config::cfg.chatbotName + "Room set to private.\0";
		_config.publicGame = false;
		return true;
	}

	/**
	 * @brief Get the prefixes
	 * @return The prefixes
	 */
	static vector<const char*> prefixes() {
		return vector<const char*> { "!private" };
	}

protected:

	/**
	 * @brief Get the internal prefixes
	 * @return The internal prefixes
	 */
	static vector<const char*> internalPrefixes() {
		return vector<const char*> { "!private " };
	}

	ParsecHostConfig& _config;
};

