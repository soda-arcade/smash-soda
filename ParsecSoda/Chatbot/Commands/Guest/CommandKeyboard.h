#pragma once

#include "parsec-dso.h"
#include "../Base/ACommandStringArg.h"
#include "../../../GamepadClient.h"

class CommandKeyboard : public ACommandStringArg
{
public:

	/**
	 * @brief Construct a new CommandKeyboard object
	 * 
	 * @param msg 
	 * @param sender 
	 * @param gamepadClient 
	 */
	CommandKeyboard(const char* msg, Guest& sender, GamepadClient& gamepadClient)
		: ACommandStringArg(msg, internalPrefixes()), _sender(sender), _gamepadClient(gamepadClient)
	{}

	/**
	 * @brief Run the command
	 * @return true if the command was successful
	 */
	bool run() override {

		// Valid button check
		if (!ACommandStringArg::run() || _gamepadClient.getKeyMap().isValidButtonName(toLower(_stringArg)) == false) {
			SetReply("See instructions at https://github.com/mickeyuk/smash-soda/wiki/keyboard");
			return false;
		}

		_gamepadClient.mapKeyboard(_sender.userID, toLower(_stringArg));
		SetReply(_sender.name + ", exit the chat window and press a key to map to " + toUpper(_stringArg) + ".\0");

		return true;

	}

	/**
	 * @brief Get the prefixes object
	 * @return vector<const char*> 
	 */
	static vector<const char*> prefixes() {
		return vector<const char*> { "!kb" };
	}

protected:
	Guest& _sender;
	GamepadClient& _gamepadClient;

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