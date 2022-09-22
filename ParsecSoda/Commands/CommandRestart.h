#pragma once

#include "ACommand.h"
#include <iostream>
#include "../CompilerDirectives.h"

using namespace std;

class CommandRestart : public ACommand
{
public:
	string filename;

	const COMMAND_TYPE type() override { return COMMAND_TYPE::BOT_MESSAGE; }

	CommandRestart() {}

	bool run() override {

		if (MetadataCache::preferences.kioskMode) {

			if (!MetadataCache::preferences.kioskRestarting) {

				_replyMessage = "[MasterHand] | Restarting yuzu...";

				//MetadataCache::preferences.kioskRestarting = true;
				_tsystem(_T("taskkill /F /T /IM yuzu.exe"));

			}
			else {

				_replyMessage = "[MasterHand] | The application is restarting...wait!";

			}

		}
		else {

			_replyMessage = "[MasterHand] | The host does not have kiosk mode enabled.";

		}
		
		return true;
	}

	static vector<const char*> prefixes() {
		return vector<const char*> { "!restart", "!kill" };
	}
};