#pragma once

#include "../Base/ACommand.h"
#include "../../../Guest.h"

class CommandStopSFX : public ACommand
{
public:

	/**
	 * @brief Construct a new CommandStopSFX object
	 * 
	 * @param sender 
	 */
	CommandStopSFX(Guest &sender)
		: _sender(sender)
	{}

	/**
	 * @brief Run the command
	 * @return true if the command was successful
	 */
	bool run() override {


		Tier tier = Cache::cache.tierList.getTier(_sender.userID);

		// SFX enabled?
		if (tier == Tier::NOOB && !Config::cfg.permissions.noob.useSFX ||
			tier == Tier::PLEB && !Config::cfg.permissions.guest.useSFX ||
			tier == Tier::MOD && !Config::cfg.permissions.moderator.useSFX ||
			tier == Tier::GOD && !Config::cfg.permissions.vip.useSFX) {
			SetReply("Sound effects are disabled.\0");
			return false;
		}
		else
		{ 
			PlaySound(NULL, NULL, SND_FILENAME | SND_NODEFAULT | SND_ASYNC | SND_SYSTEM);
		}
		return true;
	}


	/**
	 * @brief Get the prefixes
	 * @return vector<const char*> 
	 */
	static vector<const char*> prefixes() {
		return vector<const char*> { "!stop" };
	}

protected:
	Guest& _sender;
};