#pragma once

#include "ACommand.h"
#include <iostream>
#include "../TierList.h"
#include "../CompilerDirectives.h"

using namespace std;

class CommandHelp : public ACommand
{
public:
	const COMMAND_TYPE type() override { return COMMAND_TYPE::HELP; }

	CommandHelp(Guest& sender, TierList& tierList)
		: _sender(sender), _tierList(tierList)
	{}

	bool run() override
	{
		_replyMessage = Config::cfg.chatbotName + "For a full list of commands, visit: https://bit.ly/smash-soda";

		return true;
	}

	static vector<const char*> prefixes()
	{
		return vector<const char*> { "!help", "!commands" };
	}

protected:
	Guest& _sender;
	TierList& _tierList;
};

