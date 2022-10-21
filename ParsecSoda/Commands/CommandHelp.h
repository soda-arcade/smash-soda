#pragma once

#include "ACommand.h"
#include <iostream>
#include "../TierList.h"
#include "../CompilerDirectives.h"

using namespace std;

class CommandHelp : public ACommand
{
public:
	const COMMAND_TYPE type() override { return COMMAND_TYPE::HELP; }

	CommandHelp(Guest& sender, TierList& tierList)
		: _sender(sender), _tierList(tierList)
	{}

	bool run() override
	{
		/* Help list getting a bit too long

		string pleb_commands = "\n  ---- Normal Commands ----";
//#if !BASIC_VERSION
		if (!MetadataCache::preferences.basicVersion)
		{
			pleb_commands += "\n  !bonk\t\t\t\t |\tBonk another user.";
		}
//#endif
		pleb_commands += "\n  !discord\t\t\t  |\tGet invite link for host's Discord server.";
		pleb_commands += "\n  !help\t\t\t\t   |\tShow command list.";
		pleb_commands += "\n  !ff\t\t\t\t\t\t |\tDrop your gamepads.";
		pleb_commands += "\n  !mirror\t\t\t   |\tToggle mirroring of L-Stick into DPad.";
		pleb_commands += "\n  !one\t\t\t\t    |\tMaps all of your devices to the same gamepad.";
		pleb_commands += "\n  !pads\t\t\t\t   |\tShow who's holding each gamepad.";
		pleb_commands += "\n  !spectate\t\t   |\tDon't be swapped in when hotseat mode enabled.";
//#if !BASIC_VERSION
		if (!MetadataCache::preferences.basicVersion)
		{
			pleb_commands += "\n  !sfx\t\t\t\t\t  |\tPlay sound effect.";
		}
//#endif
		pleb_commands += "\n  !swap\t\t\t\t |\tReplace your gamepad with another one.";

		const string admin_commands = string()
			+ pleb_commands
			+ "\n  " + ""
			+ "\n  " + "---- Admin Commands ----"
			+ "\n  " + "!ban\t\t\t\t   |\tBan a guest."
			+ "\n  " + "!bb\t\t\t\t    |\tPress everybody's B button twice."
			+ "\n  " + "!dc\t\t\t\t    |\tDisconnect a specific gamepad."
			+ "\n  " + "!kick\t\t\t    |\tKick user from the room."
			+ "\n  " + "!strip\t\t\t   |\tStrip gamepad from player's hand."
			+ "\n  " + "!limit\t\t\t   |\tSet the maximum amount of pads a guest can hold."
			+ "\n  " + "!unban\t\t     |\tUnban a guest."
			;

		const string god_commands = string()
			+ admin_commands
			+ "\n  " + ""
			+ "\n  " + "---- God Commands ----"
			+ "\n  " + "!gameid\t\t   |\tSet game id."
			+ "\n  " + "!guests\t\t   |\tSet the amount of room slots."
			+ "\n  " + "!mic\t\t\t\t  |\tSet microphone volume."
			+ "\n  " + "!mod\t\t\t\t  |\tMake a guest in your room a moderator."
			+ "\n  " + "!name\t\t\t   |\tSet room name."
			+ "\n  " + "!private\t\t  |\tMake the room private."
			+ "\n  " + "!public\t\t   |\tMake the room public."
			+ "\n  " + "!q\t\t\t\t\t  |\tClose stream."
			+ "\n  " + "!setconfig\t  |\tApply config changes."
			+ "\n  " + "!speakers\t   |\tSet speakers volume."
			+ "\n  " + "!unmod\t\t    |\tRevoke moderator permissions."
			;

		Tier tier = _tierList.getTier(_sender.userID);
		string commandList = pleb_commands;
		if		(tier == Tier::ADMIN || tier == Tier::MOD)	commandList = admin_commands;
		else if	(tier == Tier::GOD)							commandList = god_commands;
		else												commandList = pleb_commands;

		_replyMessage =
			MetadataCache::preferences.chatbotName
			+ string(" | Command list: ")
			+ commandList
			+ "\n\0";

		*/

		_replyMessage = MetadataCache::preferences.chatbotName + "For a full list of commands, visit: https://github.com/MickeyUK/SmashSoda/wiki/Chat-Commands";

		return true;
	}

	static vector<const char*> prefixes()
	{
		return vector<const char*> { "!help", "!commands" };
	}

protected:
	Guest& _sender;
	TierList& _tierList;
};

