#pragma once

#include <sstream>
#include "../Base/ACommandIntegerArg.h"
#include "../../../AudioOut.h"

class CommandSpeakers : public ACommandIntegerArg
{
public:

	/**
	 * @brief Construct a new CommandSpeakers object
	 * 
	 * @param msg The message to parse
	 * @param audioOut The audio out
	 */
	CommandSpeakers(const char* msg, AudioOut& audioOut)
		: ACommandIntegerArg(msg, internalPrefixes()), _audioOut(audioOut)
	{}

	/**
	 * @brief Run the command
	 * @return true if the command was successful
	 */
	bool run() override {
		if (!ACommandIntegerArg::run()) {
			_replyMessage = std::string() + Config::cfg.chatbotName + "Usage: !speakers <integer in range [0, 100]>\nExample: !speakers 42\0";
			return false;
		}

		_audioOut.volume = (float)_intArg / 100.0f;
		SetReply("Speakers volume set to " + std::to_string(_intArg) + "%\0");
		return true;
	}

	/**
	 * @brief Get the prefixes
	 * @return The prefixes
	 */
	static vector<const char*> prefixes() {
		return vector<const char*> { "!speakers" };
	}

protected:
	static vector<const char*> internalPrefixes()
	{
		return vector<const char*> { "!speakers " };
	}

	AudioOut& _audioOut;
};
