#pragma once

#include "../GamepadClient.h"

class Tournament {
public:

	Tournament() {}

	enum Type {
		KOTH,
		BRACKETS
	};

	string init(Tournament::Type type, int teamSize);
	void reset();

	string shuffle();
	string brackets();
	string evaluate(int teamIndex);

	string start();
	string stop();

	bool setTeams();
	bool setRounds();
	bool setMatch();

};