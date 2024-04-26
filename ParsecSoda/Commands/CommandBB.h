#pragma once

#include <iostream>
#include "ACommand.h"
#include "../GamepadClient.h"
#include "../Modules/Macro.h"
#include "../TierList.h"
#include "../VipList.h"

class CommandBB : public ACommand
{
public:

	const COMMAND_TYPE type() override { return COMMAND_TYPE::PADS; }

	CommandBB(GamepadClient& gamepadClient, Macro& macro, TierList& tierList, VIPList vip, Guest& sender)
		: _gamepadClient(gamepadClient), _macro(macro), _tierList(tierList), _vip(vip), _sender(sender)
	{}

	bool run() override {

		Tier tier = _tierList.getTier(_sender.userID);
		if (tier == Tier::ADMIN || tier == Tier::MOD || tier == Tier::GOD || _vip.isVIP(_sender.userID)) {
			_macro.pressButtonForAll(ParsecGamepadButton::GAMEPAD_BUTTON_B);
			_macro.pressButtonForAll(ParsecGamepadButton::GAMEPAD_BUTTON_B);
			_replyMessage = Config::cfg.chatbotName + "Everybody's B button was pressed twice!\0";
		}
		else {
			_replyMessage = Config::cfg.chatbotName + "Only mods and VIPs can use this command!\0";
		}
		
		return true;
	}

	static vector<const char*> prefixes() {
		return vector<const char*> { "!bb", "!bbb" };
	}

protected:
	GamepadClient& _gamepadClient;
	Macro& _macro;
	TierList& _tierList;
	VIPList _vip;
	Guest& _sender;
};