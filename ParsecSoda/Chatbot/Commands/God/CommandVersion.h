#pragma once

#include "../../../Core/Cache.h"
#include "../Base/ACommand.h"

using namespace std;

class CommandVersion : public ACommand
{
public:

	/**
	 * @brief Construct a new CommandDiscord object
	 * 
	 * @param sender 
	 */
	CommandVersion(Guest& sender)
		: _sender(sender)
	{}

	/**
	 * @brief Run the command
	 * @return true if the command was successful
	 */
	bool run() override {
		
		SetReply(Cache::cache.version.c_str());

		return true;
	}

	/**
	 * @brief Get the prefixes object
	 * @return vector<const char*> 
	 */
	static vector<const char*> prefixes() {
		return vector<const char*> { "!v" };
	}

protected:
	Guest& _sender;
};