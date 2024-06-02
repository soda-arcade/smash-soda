#pragma once

#include "../../../Core/Config.h"
#include "../Base/ACommandSearchUser.h"
#include <Windows.h>
#include <mmsystem.h>
#include <iostream>
#include <sstream>
#include "parsec.h"
#include "../../../Helpers/Dice.h"
#include "../../../Helpers/Stopwatch.h"

class CommandBonk : public ACommandSearchUser
{
public:

	/**
	 * @brief Construct a new CommandBonk object
	 * 
	 * @param msg 
	 * @param sender 
	 * @param guests 
	 * @param host 
	 */
	CommandBonk(const char* msg, Guest& sender, GuestList& guests, Guest& host)
		: ACommandSearchUser(msg, internalPrefixes(), guests), _sender(sender), _host(host)
	{}

	/**
	 * @brief Run the command
	 * @return true if the command was successful
	 */
	bool run() override {

		// Bonk enabled?
		if (Config::cfg.chat.bonkEnabled == false) {
			SetReply("Bonk command is disabled.\0");
			return false;
		}

		if (!_stopwatch.isFinished()) {
			SetReply("Bonk command is on cooldown: " + std::to_string(_stopwatch.getRemainingTime() / 1000) + " seconds left.\0");
			return false;
		}

		ACommandSearchUser::run();

		if (_searchResult != SEARCH_USER_RESULT::FOUND)
		{
			try
			{
				if (_host.userID == stoul(_targetUsername))
				{
					_targetGuest = _host;
					_searchResult = SEARCH_USER_RESULT::FOUND;
					if (Config::cfg.chat.hostBonkProof) {
						
						srand(time(NULL));
						std::string message = _hostBonkProof[rand() % 3];
						
						std::string::size_type pos = message.find("%s");
						if (pos != std::string::npos)
							message.replace(pos, 2, _sender.name);

						SetReply(message);

						try {
							PlaySound(TEXT("./SFX/bonk-dodge.wav"), NULL, SND_FILENAME | SND_NODEFAULT | SND_ASYNC);
						}
						catch (const std::exception&) {}
					}
				}
			}
			catch (const std::exception&) {}

			if (_searchResult != SEARCH_USER_RESULT::FOUND && _targetUsername.compare(_host.name) == 0)
			{
				_targetGuest = _host;
				_searchResult = SEARCH_USER_RESULT::FOUND;
			}
		}


		bool rv = false;
		static uint8_t BONK_CHANCE = 50;

		switch (_searchResult)
		{
		case SEARCH_USER_RESULT::NOT_FOUND:
			if (Dice::roll(BONK_CHANCE))
				_replyMessage = std::string() + "[ChatBot] | " + _sender.name + " dreams of bonking but the target out of reach.\0";
			else
				_replyMessage = std::string() + "[ChatBot] | " + _sender.name + " yearns for bonking but the victim is not here.\0";
			break;

		case SEARCH_USER_RESULT::FOUND:
			_stopwatch.reset();
			rv = true;
			if (_sender.userID == _targetGuest.userID)
			{
				_replyMessage = std::string() + "[ChatBot] | " + _sender.name + " self-bonked. *Bonk!*\0";
				try
				{
					PlaySound(TEXT("./SFX/bonk-hit.wav"), NULL, SND_FILENAME | SND_NODEFAULT | SND_ASYNC);
				}
				catch (const std::exception&) {}
			}
			else if (Dice::roll(BONK_CHANCE))
			{
				_replyMessage = std::string() + "[ChatBot] | " + _sender.name + " bonked " + _targetGuest.name + ". *Bonk!*\0";
				try
				{
					PlaySound(TEXT("./SFX/bonk-hit.wav"), NULL, SND_FILENAME | SND_NODEFAULT | SND_ASYNC);
				}
				catch (const std::exception&) {}
			}
			else
			{
				_replyMessage = std::string() + "[ChatBot] | " + _targetGuest.name + " dodged " + _sender.name + "'s bonk. *Swoosh!*\0";
				try
				{
					PlaySound(TEXT("./SFX/bonk-dodge.wav"), NULL, SND_FILENAME | SND_NODEFAULT | SND_ASYNC);
				}
				catch (const std::exception&) {}
			}
			break;

		case SEARCH_USER_RESULT::FAILED:
		default:
			_replyMessage = "[ChatBot] | Usage: !bonk <username>\nExample: !bonk melon\0";
			break;
		}

		return rv;
	}

	/**
	 * @brief Get the prefixes object
	 * @return vector<const char*> 
	 */
	static vector<const char*> prefixes() {
		return vector<const char*> { "!bonk" };
	}

	/**
	 * @brief Initialize the command
	 */
	static void init();

protected:
	static vector<const char*> internalPrefixes() {
		return vector<const char*> { "!bonk " };
	}

	static Stopwatch _stopwatch;

	Guest& _sender;
	Guest& _host;

	/**
	 * @brief Bonk-proof messages
	 */
	string _hostBonkProof[3] = {
		"%s tried to bonk the host, but the host laughs at your puny efforts.\0",
		"%s tried to bonk the host, but the host's power level is 9000!!!!\0",
		"%s tried to bonk the host, but the host is just too good for you, so don't even try.\0"
	};
};