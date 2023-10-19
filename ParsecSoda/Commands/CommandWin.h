#pragma once

#include <sstream>
#include "parsec-dso.h"
#include "ACommandIntegerArg.h"
#include "../TierList.h"
#include "../Modules/Tournament.h"

class CommandWin : public ACommandIntegerArg
{
public:
	const COMMAND_TYPE type() override { return COMMAND_TYPE::BOT_MESSAGE; }

	CommandWin(const char* msg, TierList& tierList, Tournament& tournament)
		: ACommandIntegerArg(msg, internalPrefixes()), _tierList(tierList), _tournament(tournament)
	{}

	bool run() override {
		if (_intArg == 0 || _intArg > 2)
			return false;
		
		_tournament.win(_intArg);
		return true;

	}

	static vector<const char*> prefixes() {
		return vector<const char*> { "!win", "!w", "!winner" };
	}

protected:
	TierList& _tierList;
	Tournament& _tournament;

	static vector<const char*> internalPrefixes() {
		return vector<const char*> { "!win " };
	}
};