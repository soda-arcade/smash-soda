#pragma once

#include "../../../Core/Cache.h"
#include "../Base/ACommand.h"
#include "../../../GamepadClient.h"
#include "../../../Modules/Macro.h"

class CommandBB : public ACommand
{
public:

	/**
	 * @brief Construct a new CommandBB object
	 * 
	 * @param gamepadClient 
	 * @param macro 
	 * @param sender 
	 */
	CommandBB(GamepadClient& gamepadClient, Macro& macro, Guest& sender)
		: _gamepadClient(gamepadClient), _macro(macro), _sender(sender)
	{}

	/**
	 * @brief Run the command
	 * @return true if the command was successful
	 */
	bool run() override {

		Tier tier = Cache::cache.tierList.getTier(_sender.userID);
		if (tier == Tier::ADMIN || tier == Tier::MOD || tier == Tier::GOD || Cache::cache.vipList.isVIP(_sender.userID)) {
			_macro.pressButtonForAll(ParsecGamepadButton::GAMEPAD_BUTTON_B);
			_macro.pressButtonForAll(ParsecGamepadButton::GAMEPAD_BUTTON_B);
			SetReply("Everybody's B button was pressed twice!\0");
		}
		else {
			SetReply("Only mods and VIPs can use this command!\0");
		}
		
		return true;
	}

	/**
	 * @brief Get the prefixes object
	 * @return vector<const char*> 
	 */
	static vector<const char*> prefixes() {
		return vector<const char*> { "!bb", "!bbb" };
	}

protected:
	GamepadClient& _gamepadClient;
	Macro& _macro;
	Guest& _sender;
};