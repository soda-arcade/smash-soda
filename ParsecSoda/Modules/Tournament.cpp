#include "Tournament.h"

/// <summary>
/// Constructor
/// </summary>
Tournament::Tournament() : _parsec(nullptr), _guestList(nullptr), _gamepadClient(nullptr), _chatLog(nullptr), _macro(nullptr) {}

/// <summary>
/// Initialization
/// </summary>
/// <param name="gamepadClient"></param>
void Tournament::init(ParsecDSO& parsec, GuestList& guestList, GamepadClient& gamepadClient, ChatLog& chatLog, Macro& macro) {
	_parsec = &parsec;
	_guestList = &guestList;
	_gamepadClient = &gamepadClient;
	_chatLog = &chatLog;
	_macro = &macro;
}

void Tournament::create(int teamSize, bool customTeams, bool koth) {

	// Can active guests be divided in to teams?
	if (MetadataCache::preferences.activeGuests.size() % teamSize != 0) {
		_broadcastMessage("Cannot divide guests in to teams");
		return;
	}

	// Clear everything
	if (!customTeams) {
		_teams.clear();
	}
	_rounds.clear();

	// Set variables
	_teamSize = teamSize;
	isCustomTeams = customTeams;
	_activeMatch = 0;
	_activeRound = 0;
	
	_startThread = thread([&] {
	
		// Wait until gamepad client is not busy
		while (_gamepadClient->isBusy()) {
			this_thread::sleep_for(chrono::milliseconds(1000));
		}

		// Tournament now controls the gamepad client
		_gamepadClient->isSlave = true;

		// Create gamepads
		int seats = (_teamSize * 2);
		int xboxSeats = (seats > 4) ? 4 : seats;
		int psSeats = (seats >= 5) ? seats - 4 : 0;

		_gamepadClient->createGamepads(xboxSeats, psSeats);

		// Wait until ready
		while (_gamepadClient->isBusy()) {
			this_thread::sleep_for(chrono::milliseconds(1000));
		}

		// Random teams
		if (!isCustomTeams) {
			createRandomTeams();
		}

		// KOTH mode
		if (koth) {
			
		}
		else {

			// Calculate rounds total if one team is eliminated each round
			int rounds = log2(MetadataCache::preferences.activeGuests.size());
			
			// Create rounds
			for (int i = 0; i < rounds; i++) {

				// Create round
				Round round;

				// Calculate number of matches
				int matches = MetadataCache::preferences.activeGuests.size() / (pow(_teamSize, i + _teamSize));
				
				for (int j = 0; j < matches; j++) {

					// Create match
					Match match;

					// Add match to round
					round.matches.push_back(match);
				}

				// Add round to tournament
				_rounds.push_back(round);
				
			}
			
		}

		Sleep(1000);

		// Populate first round with teams
		int teamIndex = 0;
		for (int i = 0; i < _rounds[0].matches.size(); i++) {
			for (int j = 0; j < _teamSize; j++) {
				if (teamIndex >= _teams.size()) {
					break;
				}
				_rounds[0].matches[i].teams.push_back(_teams[teamIndex]);
				teamIndex++;
			}
		}
		_rounds[0].started = true;

		Sleep(1000);
		
		_broadcastMessage("\n" + brackets());

		start();
		
		_startThread.detach();
		
	});
}

void Tournament::start() {
	
	// Put team members in to gamepads
	int padIndex = 0;
	for (int i = 0; i < _rounds[_activeRound].matches[_activeMatch].teams.size(); i++) {
		for (int j = 0; j < _rounds[_activeRound].matches[_activeMatch].teams[i].members.size(); j++) {
			
			AGamepad* gamepad = _gamepadClient->getGamepad(padIndex);
			if (gamepad != nullptr) {
				gamepad->clearOwner();
				gamepad->clearState();
				
				// Find guest in guest list
				Guest target;
				_guestList->find(_rounds[_activeRound].matches[_activeMatch].teams[i].members[j].userID, &target);

				gamepad->setOwner(target, 0, false);
				
			}
			padIndex++;
			
		}
	}
	
}

void Tournament::createRandomTeams() {
	
	// Create a copy of the active guests
	vector<GuestData> guests = MetadataCache::preferences.activeGuests;

	// Shuffle guests
	random_shuffle(guests.begin(), guests.end());

	// Create teams
	for (int i = 0; i < guests.size(); i += _teamSize) {
		Team team;
		team.name = "";
		for (int j = 0; j < _teamSize; j++) {
			team.members.push_back(guests[i + j]);
		}
		_teams.push_back(team);
	}
	
}

void Tournament::createCustomTeam(vector<GuestData> members, string teamName) {
	
	Team team;
	team.name = teamName;
	team.members = members;

	_teams.push_back(team);
	
}


string Tournament::brackets() {

	ostringstream brackets;

	brackets << "-------------------" << endl;
	brackets << "Tournament Brackets" << endl;
	brackets << "-------------------" << endl;
	
	for (int i = 0; i < _rounds.size(); i++) {
		brackets << "[ROUND " << i+1 << "]" << endl;
		
		if (_rounds[i].started) {
			for (int j = 0; j < _rounds[i].matches.size(); j++) {
			brackets << "Match " << j + 1 << ": ";
			for (int k = 0; k < _rounds[i].matches[j].teams.size(); k++) {
				Team team = _rounds[i].matches[j].teams[k];
				brackets << "[";
				if (team.name != "") {
					brackets << team.name << " ";
				}
				else {
					for (int l = 0; l < team.members.size(); l++) {
						brackets << team.members[l].name;
						if (l < team.members.size() - 1) {
							brackets << " & ";
						}
					}
				}
				
				// vs
				if (k == 0) {
					brackets << "] vs ";
				}
				else {
					brackets << "]";
				}
			}

			brackets << endl;
			}
		}
		else {
			brackets << "TBD" << endl;
		}
	}

	return brackets.str();
	
}

/// <summary>
/// Formats a message.
/// </summary>
/// <param name="message">The status message</param>
/// <returns>Formatted message</returns>
void Tournament::_broadcastMessage(string message) {

	vector<Guest> guests = _guestList->getGuests();
	vector<Guest>::iterator gi;

	string msg = "[TOURNEYBOT] " + message;
	_chatLog->logMessage(msg);

	for (gi = guests.begin(); gi != guests.end(); ++gi) {
		ParsecHostSendUserData(_parsec, (*gi).id, 0, msg.c_str());
	}

}