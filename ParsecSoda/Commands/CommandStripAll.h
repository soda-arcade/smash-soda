#pragma once

#include "../Core/Config.h"
#include "ACommand.h"
#include "../GamepadClient.h"

using namespace std;

/// <summary>
/// This command strips everbodys pads.
/// </summary>
class CommandStripAll : public ACommand {
public:

	// Leave this unchanged unless you know what you're doing.
	const COMMAND_TYPE type() override { return COMMAND_TYPE::BOT_MESSAGE; }

	CommandStripAll(const char* msg, GamepadClient& gamepadClient)
		: _gamepadClient(gamepadClient) {}

	/// <summary>
	/// This is the main function that will be called when the command is run.
	/// </summary>
	/// <returns></returns>
	bool run() override {

		_gamepadClient.stripAll();
		_replyMessage = Config::cfg.chatbotName + "Stripped all pads.";

		return true;

	}

	/// <summary>
	/// These are the prefixes that will trigger the command when a user
	/// types them in chat.
	/// </summary>
	/// <returns></returns>
	static vector<const char*> prefixes() {
		return vector<const char*> { "!yoink" };
	}

protected:

	GamepadClient& _gamepadClient;

	/// <summary>
	/// Used to set the prefixes for the command when 
	/// using a string or integer argument.
	/// </summary>
	/// <returns></returns>
	static vector<const char*> internalPrefixes() {
		return vector<const char*> { "!yoink " };
	}
};