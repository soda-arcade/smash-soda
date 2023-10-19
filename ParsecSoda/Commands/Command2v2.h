#pragma once

#include <sstream>
#include "parsec-dso.h"
#include "ACommandStringArg.h"
#include "../TierList.h"
#include "../Modules/Tournament.h"

class Command2v2 : public ACommandStringArg
{
public:
	const COMMAND_TYPE type() override { return COMMAND_TYPE::BOT_MESSAGE; }

	Command2v2(const char* msg, TierList& tierList, Tournament& tournament)
		: ACommandStringArg(msg, internalPrefixes()), _tierList(tierList), _tournament(tournament)
	{}

	bool run() override {

		_tournament.create(2, false, false);
		return true;

	}

	static vector<const char*> prefixes() {
		return vector<const char*> { "!2v2" };
	}

protected:
	TierList& _tierList;
	Tournament& _tournament;

	static vector<const char*> internalPrefixes() {
		return vector<const char*> { "!2v2 " };
	}
};