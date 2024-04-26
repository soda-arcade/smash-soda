#pragma once

#include "ACommand.h"
#include <iostream>
#include "../CompilerDirectives.h"
#include "../Modules/ProcessMan.h"

using namespace std;

class CommandRestart : public ACommand
{
public:
	string filename;

	const COMMAND_TYPE type() override { return COMMAND_TYPE::BOT_MESSAGE; }

	CommandRestart() {}

	bool run() override {

		if (Config::cfg.kioskMode.enabled) {

			if (!MetadataCache::kiosk.isRestarting) {

				_replyMessage = Config::cfg.chatbotName + "Restarting application...";

			}
			else {

				_replyMessage = Config::cfg.chatbotName + "The application is restarting...wait!";

			}

		}
		else {

			_replyMessage = Config::cfg.chatbotName + "The host does not have kiosk mode enabled.";

		}
		
		return true;
	}

	static vector<const char*> prefixes() {
		return vector<const char*> { "!restart", "!kill" };
	}
};