#pragma once

#include "Macro.h"
#include "../GamepadClient.h"
#include "../Helpers/Stopwatch.h"
#include "../ChatLog.h"

/// <summary>
/// Handles the hotseat system.
/// </summary>
class Tournament {
public:

	class Team {
	public:
		vector<GuestData> members;
		string name;
	};

	class Match {
	public:
		vector<Team> teams;
		bool matchEnded = false;
		int winner;
	};

	class Round {
	public:
		vector<Match> matches;
		bool started = false;
		bool ended = false;
	};
	
	bool isCustomTeams = false;		// Are teams custom?

	Tournament();
	void init(ParsecDSO& parsec, GuestList& guestList, GamepadClient& gamepadClient, ChatLog& chatLog, Macro& macro);

	void create(int teamSize, bool customTeams, bool koth);		// Starts the tournament system
	void start();				// Starts the tournament
	void win(int teamIndex);	// Ends the match and declares a winner
	
	void createRandomTeams();		// Creates random teams
	void createCustomTeam(vector<GuestData> members, string teamName = "");		// Creates custom team
	string brackets();		// Show tournament brackets in chat
	

private:
	ParsecDSO* _parsec;				// Parsec DSO (for sending chat messages)
	GuestList* _guestList;			// Guest list (for getting guest names)
	GamepadClient* _gamepadClient;	// Gamepad client (for controlling gamepads)
	ChatLog* _chatLog;				// Chat log (for displaying log messages)
	Macro* _macro;					// Macro (for automating button presses)

	thread _startThread;			// Thread for starting the tournament system

	vector<Team> _teams;
	vector<Round> _rounds;

	int _teamSize;					// Size of each team
	int _activeRound;				// Index of the active round
	int _activeMatch;				// Index of the active match

	void _broadcastMessage(string message);		// Broadcasts a message to all guests
	void _startMatch();							// Starts the active match
	
};