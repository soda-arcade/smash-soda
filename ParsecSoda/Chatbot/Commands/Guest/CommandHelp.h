#pragma once

#include "../Base/ACommand.h"
#include "../../../Guest.h"

using namespace std;

class CommandHelp : public ACommand
{
public:

	/**
	 * @brief Construct a new CommandHelp object
	 * 
	 * @param sender 
	 */
	CommandHelp(Guest& sender)
		: _sender(sender)
	{}

	/**
	 * @brief Run the command
	 * @return true if the command was successful
	 */
	bool run() override {
		//_replyMessage = Config::cfg.chatbotName + "For a full list of commands, visit: https://bit.ly/smash-soda";
		return true;
	}

	/**
	 * @brief Get the prefixes object
	 * @return vector<const char*> 
	 */
	static vector<const char*> prefixes() {
		return vector<const char*> { "!help", "!commands" };
	}

protected:
	Guest& _sender;
};

