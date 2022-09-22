#pragma once

#include <algorithm>
#include <random>
#include "parsec-dso.h"
#include "ACommandStringArg.h"

class CommandTeams : public ACommandStringArg
{
public:
	const COMMAND_TYPE type() override { return COMMAND_TYPE::BOT_MESSAGE; }

	CommandTeams(const char* msg, Guest& sender, GuestList& guests, GamepadClient& gamepadClient, TierList& tierList)
		: ACommandStringArg(msg, internalPrefixes()), _sender(sender), _guests(guests), _gamepadClient(gamepadClient), _tierList(tierList)
	{}

	bool run() override {

		// Get guest permissions
		_tier = _tierList.getTier(_sender.userID);

		if (!ACommandStringArg::run())
			return init();

		if (_stringArg == "start") return start();
		if (_stringArg == "stop") return stop();
		if (_stringArg == "shuffle") return shuffle();
		if (_stringArg == "brackets") {
			_replyMessage = "[TeamBot]\n" + brackets();
			return true;
		}

		return false;

	}

	static vector<const char*> prefixes() {
		return vector<const char*> { "!teams" };
	}

	bool init() {

		// Has permission to use this command?
		if (_tier == Tier::PLEB) return false;

		// Do we have enough players?
		if (_guests.getGuests().size() < 4) {
			_replyMessage = "[TeamBot] | We need at least 4 guests for a 2v2 tournament!";
			return false;
		}

		// Can the guest list be divided evenly?
		if (_guests.getGuests().size() % 2 != 0) {
			_replyMessage = "[TeamBot] | Odd number of guests, can't make fair teams!";
			return false;
		}

		// Add all participants
		MetadataCache::teams.guests = vector<string>();
		for (int i = 0; i < _guests.getGuests().size(); i++)
			MetadataCache::teams.guests.push_back(_guests.getGuests()[i].name);

		// Create teams
		shuffle();

		// Confirmation and show brackets
		_replyMessage = "[TeamBot] | Created a new doubles tournament\nUse \"!teams start\" to begin!\n\n" + brackets() + "\0";

		return true;

	}

	bool start() {

		// Has permission to use this command?
		if (_tier == Tier::PLEB) return false;

		return true;
	}

	bool stop() {

		// Has permission to use this command?
		if (_tier == Tier::PLEB) return false;

		// Clear everything
		MetadataCache::Teams::reset();

		return true;
	}

	bool shuffle() {

		// Has permission to use this command?
		if (_tier == Tier::PLEB) return false;

		// Can't shuffle if tournament has already started!
		if (MetadataCache::teams.isStarted) {
			_replyMessage = "[TeamBot] Can't shuffle the teams, the tournament has started!";
			return false;
		}

		// Clear
		MetadataCache::teams.teams = vector<MetadataCache::Teams::Team>();
		MetadataCache::teams.rounds = vector<MetadataCache::Teams::Round>();

		// Shuffle participant list
		for (int i = 0; i < 8; i += 2) {

			// New team
			MetadataCache::Teams::Team team = MetadataCache::Teams::Team();

			// Add members
			team.guests.push_back(random_string(5));
			team.guests.push_back(random_string(5));

			// Add team to the tournament
			MetadataCache::teams.teams.push_back(team);

		}
		/*
		auto rng = std::default_random_engine{};
		std::shuffle(std::begin(MetadataCache::teams.guests), std::end(MetadataCache::teams.guests), rng);

		// Now we create teams
		for (int i = 0; i < MetadataCache::teams.guests.size(); i+=2) {

			// New team
			MetadataCache::Teams::Team team = MetadataCache::Teams::Team();

			// Add members
			team.guests.push_back(MetadataCache::teams.guests[i]);
			team.guests.push_back(MetadataCache::teams.guests[i+1]);

			// Add team to the tournament
			MetadataCache::teams.teams.push_back(team);

		}
		*/

		// Create first round
		int brackets = (MetadataCache::teams.teams.size() / 2);
		createRound(0, brackets);

		// Create new rounds
		int teamIndex = 2;
		for (int i = brackets-1; i > 0; i--) {
			createRound(teamIndex, i);
			teamIndex++;
		}

		return true;
	}

	string brackets() {
		return "";
	}

	void createRound(int teamIndex, int brackets) {

		// Create new round
		MetadataCache::Teams::Round round = MetadataCache::Teams::Round();

		// Create brackets
		for (int i = 0; i < brackets; i++) {

			// New bracket
			MetadataCache::Teams::Bracket bracket = MetadataCache::Teams::Bracket();

			// Add 2 teams
			bracket.teams.push_back(MetadataCache::teams.teams[teamIndex]);
			bracket.teams.push_back(MetadataCache::teams.teams[teamIndex+1]);

			// Add bracket to round
			round.brackets.push_back(bracket);

		}

		// Add round to rounds
		MetadataCache::teams.rounds.push_back(round);

	}

	std::string random_string(size_t length) {
		auto randchar = []() -> char
		{
			const char charset[] =
				"0123456789"
				"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
				"abcdefghijklmnopqrstuvwxyz";
			const size_t max_index = (sizeof(charset) - 1);
			return charset[rand() % max_index];
		};
		std::string str(length, 0);
		std::generate_n(str.begin(), length, randchar);
		return str;
	}

protected:
	Guest& _sender;
	GuestList& _guests;
	GamepadClient& _gamepadClient;
	TierList& _tierList;

	Tier _tier;

	static vector<const char*> internalPrefixes() {
		return vector<const char*> { "!teams " };
	}
};
