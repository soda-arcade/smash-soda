#pragma once

#include "../../../Core/Cache.h"
#include "../Base/ACommand.h"

using namespace std;

class CommandUnbanLastIP : public ACommand {
public:

	CommandUnbanLastIP(const char* msg) {}

	/**
	 * @brief Run the command
	 * This is the main function that will be called when the command is run.
	 *
	 * @return true
	 */
	bool run() override {

		if (Cache::cache.isBannedIPAddress(Cache::cache.lastIpAddress)) {
			_replyMessage = "Unbanning the last IP address.";
			Cache::cache.unbanIPAddress(Cache::cache.lastIpAddress);
		} else {
			_replyMessage = "The last IP address is not banned.";
		}

		return true;

	}

	/**
	 * @brief Get the prefixes object
	 * These are the prefixes that will trigger the command when a user types in chat.
	 *
	 * @return vector<const char*>
	 */
	static vector<const char*> prefixes() {
		return vector<const char*> { "!unbanlastip" };
	}

protected:

	/**
	 * @brief Get the internal prefixes object
	 * Used to set the prefixes for the command when using a string or integer argument.
	 *
	 * @return vector<const char*>
	 */
	static vector<const char*> internalPrefixes() {
		return vector<const char*> { "!unbanlastip " };
	}
};