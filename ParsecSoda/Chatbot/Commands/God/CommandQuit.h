#pragma once

#include "../Base/ACommand.h"

class CommandQuit : public ACommand
{
public:

	/**
	 * @brief Construct a new CommandQuit object
	 * 
	 * @param hostingLoopController The hosting loop controller
	 */
	CommandQuit(bool & hostingLoopController)
		: _hostingLoopController(hostingLoopController)
	{}

	/**
	 * @brief Run the command
	 * @return true if the command was successful
	 */
	bool run() override {
		_replyMessage = Config::cfg.chatbotName + "Closing stream...\0";
		_hostingLoopController = false;
		return true;
	}

	/**
	 * @brief Get the prefixes
	 * @return The prefixes
	 */
	static vector<const char*> prefixes() {
		return vector<const char*> { "!q" };
	}

protected:
	bool& _hostingLoopController;
};

