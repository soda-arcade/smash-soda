#pragma once

#include "ACommand.h"
#include <iostream>
#include "../CompilerDirectives.h"
#include "../Modules/Hotseat.h"

using namespace std;

class CommandHotseat : public ACommand
{
public:
	const COMMAND_TYPE type() override { return COMMAND_TYPE::SETCONFIG; }

	CommandHotseat(Guest& sender, Hotseat& hotseat)
		: _sender(sender), _hotseat(hotseat)
	{}

	bool run() override {

		if (Config::cfg.hotseat.enabled) {
			_replyMessage = Config::cfg.chatbotName + " hotseat has been disabled.";
			Config::cfg.hotseat.enabled = false;
		}
		else {
			_replyMessage = Config::cfg.chatbotName + " hotseat has been enabled.";
			Config::cfg.hotseat.enabled = true;
		}

		return true;

	}

	static vector<const char*> prefixes() {
		return vector<const char*> { "!hotseat", "!hotseatoff", "!hotseaton" };
	}

protected:
	Guest& _sender;
	Hotseat& _hotseat;
};