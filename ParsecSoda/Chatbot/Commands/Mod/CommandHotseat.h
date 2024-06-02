#pragma once

#include "../Base/ACommand.h"
#include "../../../Modules/Hotseat.h"

using namespace std;

class CommandHotseat : public ACommand
{
public:

	/**
	 * @brief Construct a new CommandHotseat object
	 * 
	 * @param sender
	 * @param hotseat
	 */
	CommandHotseat(Guest& sender, Hotseat& hotseat)
		: _sender(sender), _hotseat(hotseat)
	{}

	/**
	 * @brief Run the command
	 * 
	 * @return true
	 * @return false
	 */
	bool run() override {

		if (Config::cfg.hotseat.enabled) {
			SetReply("Hotseat has been disabled.");
			Config::cfg.hotseat.enabled = false;
		}
		else {
			SetReply("Hotseat has been enabled.");
			Config::cfg.hotseat.enabled = true;
		}

		return true;

	}

	/**
	 * @brief Get the prefixes object
	 * 
	 * @return std::vector<const char*>
	 */
	static vector<const char*> prefixes() {
		return vector<const char*> { "!hotseat", "!hotseatoff", "!hotseaton" };
	}

protected:
	Guest& _sender;
	Hotseat& _hotseat;
};