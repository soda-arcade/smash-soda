#pragma once

#include "parsec-dso.h"
#include "../Base/ACommand.h"
#include <iostream>

class CommandSetConfig : public ACommand
{
public:

	/**
	 * @brief Construct a new CommandSetConfig object
	 * 
	 * @param parsec The Parsec DSO
	 * @param config The host config
	 * @param sessionId The session ID
	 */
	CommandSetConfig(ParsecDSO* parsec, ParsecHostConfig* config, const char* sessionId)
		: _parsec(parsec), _config(config), _sessionId(sessionId)
	{}

	/**
	 * @brief Run the command
	 * @return true if the command was successful
	 */
	bool run() override {
		_replyMessage = Config::cfg.chatbotName + "Room settings applied.\0";
		ParsecHostSetConfig(_parsec, _config, _sessionId);
		return true;
	}

	/**
	 * @brief Get the prefixes
	 * @return The prefixes
	 */
	static vector<const char*> prefixes() {
		return vector<const char*> { "!setconfig" };
	}

protected:
	ParsecDSO* _parsec;
	ParsecHostConfig* _config;
	const char* _sessionId;
};