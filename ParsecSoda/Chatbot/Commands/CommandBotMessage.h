#pragma once

#include "Base/ACommand.h"

class CommandBotMessage : public ACommand
{
public:

	/**
	 * @brief Construct a new CommandBotMessage object
	 * 
	 * @param msg
	 */
	CommandBotMessage(const char* msg)
		: _msg(msg)
	{}

	/**
	 * @brief Run the command
	 * 
	 * @return true
	 * @return false
	 */
	bool run() override {
		SetReply(_msg.c_str());
		return true;
	}

protected:
	string _msg;
};