#pragma once

#include "../Base/ACommand.h"

using namespace std;

class CommandDiscord : public ACommand
{
public:

	/**
	 * @brief Construct a new CommandDiscord object
	 * 
	 * @param sender 
	 */
	CommandDiscord(Guest& sender)
		: _sender(sender)
	{}

	/**
	 * @brief Run the command
	 * @return true if the command was successful
	 */
	bool run() override {
		
		if (Config::cfg.chat.discord != "") {
			SetReply(Config::cfg.chat.discord.c_str());
		} else return false;

		return true;
	}

	/**
	 * @brief Get the prefixes object
	 * @return vector<const char*> 
	 */
	static vector<const char*> prefixes() {
		return vector<const char*> { "!discord" };
	}

protected:
	Guest& _sender;
};

