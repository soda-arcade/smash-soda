#include "Tournament.h"

/// <summary>
/// Sets up a new tournament.
/// </summary>
/// <param name="type">Type of tournament (KoTH or brackets)</param>
/// <param name="teamSize">Number of players in a team</param>
/// <returns>Tournament status</returns>
string Tournament::setup(int tournamentType, int setTeamSize) {

	// Clears everything for new tournament
	reset();

	// Tournament properties
	MetadataCache::tournament.type = tournamentType;
	MetadataCache::tournament.teamSize = setTeamSize;

	// Enough players?
	if (!setPlayers()) return msg("Not enough players for the tournament!");

	// Valid teams?
	if (!setTeams()) return msg("Not enough players to create fair teams!");

	// Create rounds and brackets
	if (!setRounds()) return msg("Something went wrong and tournament couldn't be started!");

	// Good to go!
	return msg("Tournament is ready! Do !start to begin!\n" + brackets());

}

/// <summary>
/// Resets tournament system to default state.
/// </summary>
void Tournament::reset() {

}

/// <summary>
/// Create a list of active players for the tournament.
/// Aborts tournament if player count not valid.
/// </summary>
/// <returns>succesful?</returns>
bool Tournament::setPlayers() {

	// King of the hill?
	if (MetadataCache::tournament.type == 0) return true;

	// Number of guests in room
	int guestListSize = MetadataCache::preferences.activeGuests.size();

	// Do we have enough guests in room for single match?
	if (guestListSize < (MetadataCache::tournament.teamSize * 2)) return false;

	// Can players be divided equally in to teams?
	if (guestListSize % MetadataCache::tournament.teamSize != 0) return false;

	return true;

}

/// <summary>
/// Randomly generates teams from the tournament players
/// list, and aborts if valid teams can't be created.
/// </summary>
/// <returns>successful?</returns>
bool Tournament::setTeams() {

	// Shuffle active guest list for random teams
	std::shuffle(MetadataCache::preferences.activeGuests.begin(), 
		MetadataCache::preferences.activeGuests.end(), std::mt19937{ std::random_device{}() });

	// Create teams
	for (int i = 0; i < MetadataCache::preferences.activeGuests.size(); i+=MetadataCache::tournament.teamSize) {

		MetadataCache::Tournament::Team team = MetadataCache::Tournament::Team();

		team.guests.push_back(MetadataCache::preferences.activeGuests[i]);

		if (MetadataCache::tournament.teamSize == 2)
			team.guests.push_back(MetadataCache::preferences.activeGuests[i+1]);

		MetadataCache::tournament.teams.push_back(team);

	}

	return true;

}

/// <summary>
/// Creates the rounds and brackets for the tournament,
/// and aborts if something goes wrong.
/// </summary>
/// <returns>succesful?</returns>
bool Tournament::setRounds() {

	// Create rounds
	int rounds = log2(MetadataCache::preferences.activeGuests.size());
	int nBrackets = MetadataCache::tournament.teams.size() / 2;

	for (int i = 0; i < rounds; i++) {

		// New round
		MetadataCache::Tournament::Round round = MetadataCache::Tournament::Round();

		// Create brackets for that round
		for (int j = 0; j < nBrackets; j++) {

			// New bracket
			MetadataCache::Tournament::Bracket bracket = MetadataCache::Tournament::Bracket();
			round.brackets.push_back(bracket);
			nBrackets = floor(nBrackets / 2);

		}

		// First round
		if (i == 0) {

			// Populate first round
			int nTeams = 0;
			for (int j = 0; j < round.brackets.size(); j++) {

				// Add teams to bracket
				round.brackets[j].teams.push_back(MetadataCache::tournament.teams[nTeams]);
				round.brackets[j].teams.push_back(MetadataCache::tournament.teams[nTeams + 1]);
				nTeams++;

			}

		}

		// Add round
		MetadataCache::tournament.rounds.push_back(round);

	}

	// Round 1 ready to start
	MetadataCache::tournament.rounds[0].isStarted = true;

	return true;

}

/// <summary>
/// Sets up a tournament match and assigns the pads to the
/// players.
/// </summary>
/// <returns>successful?</returns>
bool Tournament::setMatch() {

	return true;

}

/// <summary>
/// Regenerates the teams.
/// </summary>
/// <returns>The new brackets</returns>
string Tournament::shuffle() {

	return brackets();

}

/// <summary>
/// Displays the current tournament standings.
/// </summary>
/// <returns>Brackets</returns>
string Tournament::brackets() {

	string brackets = "";

	brackets += "-----------------\n";

	for (int i = 0; i < MetadataCache::tournament.rounds.size(); i++) {

		if (i == MetadataCache::tournament.rounds.size() - 1) {
			brackets += "------------------\n";
			brackets += "| WINNNERS |\n";
			brackets += "------------------\n";
		}
		else {
			brackets += "--------------\n";
			brackets += "| ROUND " + std::to_string(i + 1) + " |\n";
			brackets += "--------------\n";
		}

		if (MetadataCache::tournament.rounds[i].isStarted) {

			// For each bracket
			for (int j = 0; j < MetadataCache::tournament.rounds[i].brackets.size(); j++) {

				brackets += "[";
				brackets += MetadataCache::tournament.rounds[i].brackets[j].teams[0].guests[0].name;
				brackets += " & ";
				brackets += MetadataCache::tournament.rounds[i].brackets[j].teams[0].guests[1].name;
				brackets += "]";

				brackets += " vs ";

				brackets += "[";
				brackets += MetadataCache::tournament.rounds[i].brackets[j].teams[1].guests[0].name;
				brackets += " & ";
				brackets += MetadataCache::tournament.rounds[i].brackets[j].teams[1].guests[1].name;
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
/// Evaluates the match results, the current tournament
/// standings and sets up the next match (or declare winner).
/// </summary>
/// <param name="teamIndex">The winning team/player of current match</param>
/// <returns>Tournament status</returns>
string Tournament::evaluate(int teamIndex) {

	return "";

}

/// <summary>
/// Starts a tournament once it has been setup.
/// </summary>
/// <returns>Tournament status</returns>
string Tournament::start() {

	return "";

}

/// <summary>
/// Stops a tournament in progress.
/// </summary>
/// <returns>Tournament status</returns>
string Tournament::stop() {

	return "";

}

/// <summary>
/// Formats a status message.
/// </summary>
/// <param name="message">The status message</param>
/// <returns>Formatted message</returns>
string Tournament::msg(string message) {

	return "[TourneyBot] " + message;

}