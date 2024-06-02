#pragma once

#include "parsec-dso.h"
#include "../Base/ACommand.h"
#include <iostream>

class CommandPublic : public ACommand
{
public:

	/**
	 * @brief Construct a new CommandPublic object
	 * 
	 * @param config The host config
	 */
	CommandPublic(ParsecHostConfig& config)
		: _config(config)
	{}

	/**
	 * @brief Run the command
	 * @return true if the command was successful
	 */
	bool run() override {
		_replyMessage = Config::cfg.chatbotName + "Room set to public.\0";
		_config.publicGame = true;
		return true;
	}

	/**
	 * @brief Get the prefixes
	 * @return The prefixes
	 */
	static vector<const char*> prefixes(){
		return vector<const char*> { "!public" };
	}

protected:

	/**
	 * @brief Get the internal prefixes
	 * @return The internal prefixes
	 */
	static vector<const char*> internalPrefixes() {
		return vector<const char*> { "!public " };
	}

	ParsecHostConfig& _config;
};

