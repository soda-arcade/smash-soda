#pragma once

#include "../Base/ACommand.h"
#include "../../../Modules/Hotseat.h"
#include "../../../GamepadClient.h"

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
	CommandHotseat(Guest& sender, Hotseat& hotseat, GamepadClient& gamepadClient)
		: _sender(sender), _hotseat(hotseat), _gamepadClient(gamepadClient)
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
			Hotseat::instance.Stop();
		}
		else {
			SetReply("Hotseat has been enabled.");
			Config::cfg.hotseat.enabled = true;
			Hotseat::instance.Start();
			for (size_t i = 0; i < _gamepadClient.gamepads.size(); ++i)
			{
				if (_gamepadClient.gamepads[i]->isOwned())
				{
					Guest guest = _gamepadClient.gamepads[i]->owner.guest;
					Hotseat::instance.checkUser(guest.id, guest.name);
				}
			}
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
	GamepadClient& _gamepadClient;
};