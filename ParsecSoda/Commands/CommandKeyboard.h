#pragma once

#include <iostream>
#include "parsec-dso.h"
#include "ACommandStringArg.h"
#include "../TierList.h"
#include "../GamepadClient.h"

class CommandKeyboard : public ACommandStringArg
{
public:
	const COMMAND_TYPE type() override { return COMMAND_TYPE::SFX; }

	CommandKeyboard(const char* msg, Guest& sender, GamepadClient& gamepadClient, TierList& tierList)
		: ACommandStringArg(msg, internalPrefixes()), _sender(sender), _gamepadClient(gamepadClient), _tierList(tierList)
	{}

	bool run() override {

		// Valid button check
		if (!ACommandStringArg::run() || _gamepadClient.getKeyMap().isValidButtonName(toLower(_stringArg)) == false) {
			_replyMessage = Config::cfg.chatbotName;
			_replyMessage += "See instructions at https://github.com/mickeyuk/smash-soda/wiki/keyboard";
			return false;
		}

		_gamepadClient.mapKeyboard(_sender.userID, toLower(_stringArg));

		_replyMessage = Config::cfg.chatbotName;
		_replyMessage += _sender.name + ", press a key to map to " + toUpper(_stringArg) + " on your keyboard.";

		return true;

	}

	static vector<const char*> prefixes() {
		return vector<const char*> { "!kb" };
	}

protected:
	Guest& _sender;
	GamepadClient& _gamepadClient;
	TierList& _tierList;

	// Split msg into vector of strings by space
	vector<string> splitMsg() {
		vector<string> result;
		stringstream ss(_msg);
		string item;
		while (getline(ss, item, ' ')) {
			result.push_back(item);
		}
		return result;
	}

	// Convert string to lowercase
	string toLower(string str) {
		transform(str.begin(), str.end(), str.begin(), ::tolower);
		return str;
	}

	// Convert string to uppercase
	string toUpper(string str) {
		transform(str.begin(), str.end(), str.begin(), ::toupper);
		return str;
	}

	static vector<const char*> internalPrefixes() {
		return vector<const char*> { "!kb " };
	}
};