#pragma once

#include "parsec-dso.h"
#include "ACommandStringArg.h"
#include <algorithm>
#include <random>

class Command2v2 : public ACommandStringArg {
public:
	const COMMAND_TYPE type() override { return COMMAND_TYPE::BOT_MESSAGE; }

	Command2v2(const char* msg, GuestList& guests, GamepadClient& gamepadClient)
		: ACommandStringArg(msg, internalPrefixes()), _gamepadClient(gamepadClient)
	{}

	/// <summary>
	/// When command is first executed.
	/// </summary>
	/// <returns>Boolean</returns>
	bool run() override {

		// No sub command given
		if (!ACommandStringArg::run()) {

			// Start tournament
			if (!MetadataCache::teams.isInitiated) {		
				return initTournament();
			}
			else {
				_replyMessage = "[2v2Bot] | Usage: https://github.com/MickeyUK/SmashSoda/wiki/Tournaments\0";
				return false;
			}
			
		}

		return true;
	}

	/// <summary>
	/// Initializes the 2v2 Tournament.
	/// </summary>
	bool initTournament() {

		// It has begun...
		MetadataCache::teams.isInitiated = true;

		// Generate some teams
		if (!createTeams()) {
			_replyMessage = "[2v2Bot] | We don't have enough players for a fair tournament!\0";
			return false;
		} else

		// Create rounds and brackets for tournament
		if (!createRounds()) {
			_replyMessage = "[2v2Bot] | Something went wrong and the tournament could not be created!\0";
			return false;
		} else 

		if (startTournament()) {

			string msg = "[2v2Bot] | A 2v2 tournament has started!\n";
			msg += showBrackets();
			_replyMessage = msg;
			return true;

		} else return false;

	}

	/// <summary>
	/// Automatically creates teams from the guest list.
	/// </summary>
	bool createTeams() {

		// Clear guest list
		MetadataCache::teams.guests.clear();
		MetadataCache::teams.guests.erase(MetadataCache::teams.guests.begin(), MetadataCache::teams.guests.end());
		MetadataCache::teams.guests.shrink_to_fit();

		// Clear team list
		MetadataCache::teams.teams.clear();

		// Create participants from guest list
		// TODO
		// Dummy guests list
		MetadataCache::teams.guests.push_back("MickeyUK");
		MetadataCache::teams.guests.push_back("bigboi83");
		MetadataCache::teams.guests.push_back("RefffiK");
		MetadataCache::teams.guests.push_back("oscar");
		MetadataCache::teams.guests.push_back("Irish-Bstard");
		MetadataCache::teams.guests.push_back("Dr. Puff");
		MetadataCache::teams.guests.push_back("AlexErBest");
		MetadataCache::teams.guests.push_back("Altearis");
		MetadataCache::teams.guests.push_back("Sniggles");
		MetadataCache::teams.guests.push_back("Ruffio");
		MetadataCache::teams.guests.push_back("WTF");
		MetadataCache::teams.guests.push_back("Random");
		MetadataCache::teams.guests.push_back("lolz");
		MetadataCache::teams.guests.push_back("dunno");
		MetadataCache::teams.guests.push_back("something");
		MetadataCache::teams.guests.push_back("teehee");

		// Do we have enough guests?
		if (MetadataCache::teams.guests.size() % 2 != 0) return false;

		// Shuffle the guest list
		std::shuffle(MetadataCache::teams.guests.begin(), MetadataCache::teams.guests.end(), std::mt19937{ std::random_device{}() });

		// Make teams
		for (int i = 0; i < MetadataCache::teams.guests.size(); i+=2) {

			// Create a new team
			MetadataCache::Teams::Team team = MetadataCache::Teams::Team();

			// Add members
			team.guests.push_back(&MetadataCache::teams.guests[i]);
			team.guests.push_back(&MetadataCache::teams.guests[i+1]);

			// Add team to teams list
			MetadataCache::teams.teams.push_back(team);

		}

		return true;

	}

	bool createRounds() {

		// Create rounds
		int rounds = log2(MetadataCache::teams.guests.size());
		int nBrackets = MetadataCache::teams.teams.size() / 2;

		for (int i = 0; i < rounds; i++) {

			// New round
			MetadataCache::Teams::Round round = MetadataCache::Teams::Round();

			// Create brackets for that round
			int nBrackets = log2(nBrackets / 2);
			for (int j = 0; j < nBrackets; j++) {

				// New bracket
				MetadataCache::Teams::Bracket bracket = MetadataCache::Teams::Bracket();
				round.brackets.push_back(bracket);
				nBrackets = floor(nBrackets / 2);

			}

			// Add round
			MetadataCache::teams.rounds.push_back(round);

		}

		return true;

	}

	bool startTournament() {

		// Populate first round
		int nTeams = 0;
		for (int i = 0; i < MetadataCache::teams.rounds[0].brackets.size(); i++) {

			// Add teams to bracket
			MetadataCache::teams.rounds[0].brackets[i].teams.push_back(MetadataCache::teams.teams[nTeams]);
			MetadataCache::teams.rounds[0].brackets[i].teams.push_back(MetadataCache::teams.teams[nTeams+1]);
			nTeams++;

		}

		// Tournament has started
		MetadataCache::teams.isStarted = true;
		MetadataCache::teams.rounds[0].isStarted = true;

		// Prepare gamepads
		//_gamepadClient.disconnectAllGamepads();
		//_gamepadClient.getGamepad(0)->connect();
		//_gamepadClient.getGamepad(1)->connect();

		return true;

	}

	string showBrackets() {

		string brackets = "";

		brackets += "-----------------\n";

		for (int i = 0; i < MetadataCache::teams.rounds.size(); i++) {

			if (i == MetadataCache::teams.rounds.size() - 1) {
				brackets += "------------------\n";
				brackets += "| WINNNERS |\n";
				brackets += "------------------\n";
			}
			else {
				brackets += "--------------\n";
				brackets += "| ROUND " + std::to_string(i + 1) + " |\n";
				brackets += "--------------\n";
			}

			if (MetadataCache::teams.rounds[i].isStarted) {
				
				// For each bracket
				for (int j = 0; j < MetadataCache::teams.rounds[i].brackets.size(); j++) {

					brackets += "[";
					brackets += MetadataCache::teams.rounds[i].brackets[j].teams[0].guests[0]->c_str();
					brackets += " & ";
					brackets += MetadataCache::teams.rounds[i].brackets[j].teams[0].guests[1]->c_str();
					brackets += "]";

					brackets += " vs ";

					brackets += "[";
					brackets += MetadataCache::teams.rounds[i].brackets[j].teams[1].guests[0]->c_str();
					brackets += " & ";
					brackets += MetadataCache::teams.rounds[i].brackets[j].teams[1].guests[1]->c_str();
					brackets += "]\n";

				}

			}
			else {

				brackets += "TBD\n";

			}

			brackets += "\n";

		}

		brackets += "\0";
		return brackets;

	}

	/// <summary>
	/// Command prefixes.
	/// </summary>
	/// <returns>Vector</returns>
	static vector<const char*> prefixes() {
		return vector<const char*> { "!2v2" };
	}

protected:

	GamepadClient& _gamepadClient;

	/// <summary>
	/// Internal prefixes.
	/// </summary>
	/// <returns>Vector</returns>
	static vector<const char*> internalPrefixes() {
		return vector<const char*> { "!2v2 " };
	}

};