#pragma once

#include "../../../Core/Config.h"
#include "../../../Core/Cache.h"
#include "parsec-dso.h"
#include "../Base/ACommandStringArg.h"
#include "../../../SFXList.h"

class CommandSFX : public ACommandStringArg
{
public:
	
	/**
	 * @brief Construct a new CommandSFX object
	 * 
	 * @param sender 
	 */
	CommandSFX(const char* msg, Guest& sender, SFXList &sfxList)
		: ACommandStringArg(msg, internalPrefixes()), _sender(sender), _sfxList(sfxList)
	{}

	/**
	 * @brief Run the command
	 * 
	 * @return true 
	 * @return false 
	 */
	bool run() override {

		// Tier list enabled?
		Tier tier = Cache::cache.tierList.getTier(_sender.userID);

		// SFX enabled?
		if (tier == Tier::PLEB && !Config::cfg.permissions.guest.useSFX ||
			tier == Tier::MOD && !Config::cfg.permissions.moderator.useSFX ||
			tier == Tier::GOD && !Config::cfg.permissions.vip.useSFX) {
			SetReply("Sound effects are disabled.\0");
			return false;
		}

		if (_sfxList.size() <= 0) {
			SetReply("No sound effects available.\0");
			return false;
		}

		if ( !ACommandStringArg::run() )
		{
			_replyMessage =
				Config::cfg.chatbotName +
				string("Usage: !sfx <sound name> | Example: !sfx bruh\n") +
				string("List of available sound names:\n") +
				_sfxList.loadedTags() +
				string("\0");
			return false;
		}

		SFXList::SFXPlayResult result = _sfxList.play(_stringArg);

		switch (result)
		{
		case SFXList::SFXPlayResult::COOLDOWN:
			_replyMessage =
				Config::cfg.chatbotName +
				string(" | Command !sfx is on cooldown: ") +
				to_string(_sfxList.getRemainingCooldown()) +
				string(" seconds left.");
			break;
		case SFXList::SFXPlayResult::NOT_FOUND:
			_replyMessage =
				Config::cfg.chatbotName +
				string(" | This sound does not exist.\n") +
				string("List of available sound effects:\n") +
				_sfxList.loadedTags() +
				string("\0");
			break;
		case SFXList::SFXPlayResult::OK:
		default:
			_replyMessage = "";
			break;
		}

		return true;
	}

	/**
	 * @brief Get the prefixes object
	 * 
	 * @return vector<const char*> 
	 */
	static vector<const char*> prefixes() {
		return vector<const char*> { "!sfx" };
	}

protected:
	static vector<const char*> internalPrefixes()
	{
		return vector<const char*> { "!sfx " };
	}
	Guest& _sender;
	SFXList& _sfxList;
};
