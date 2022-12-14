#pragma once

#include "../GamepadClient.h"
#include <functional>

class Tournament {
public:

	string setup(int tournamentType, int setTeamSize);
	void reset();

	string shuffle();
	string brackets();
	string evaluate(int teamIndex);

	string start();
	string stop();

private:

	bool setPlayers();
	bool setTeams();
	bool setRounds();
	bool setMatch();

	string msg(string message);

};