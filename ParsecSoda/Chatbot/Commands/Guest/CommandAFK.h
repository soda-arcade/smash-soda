#pragma once

#include "../../../GamepadClient.h"
#include "../../../GuestList.h"

class CommandAFK : public ACommand
{
public:

	/**
	 * @brief Construct a new CommandAFK object
	 * 
	 * @param guests 
	 * @param padClient 
	 */
	CommandAFK(GuestList &guests, GamepadClient& padClient)
		: _guests(guests), _padClient(padClient)
	{}

	/**
	 * @brief Run the command
	 * @return true if the command was successful
	 */
	bool run() override {
		int clearCount = _padClient.clearAFK(_guests);
		SetReply("Retrieving AFK gamepads... Gamepads cleared:  " + std::to_string(clearCount) + "\0");
		return true;
	}

	/**
	 * @brief Get the prefixes object
	 * @return vector<const char*> 
	 */
	static vector<const char*> prefixes() {
		return vector<const char*> { "!afk" };
	}

private:
	GuestList &_guests;
	GamepadClient &_padClient;
};
