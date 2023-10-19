#pragma once

#include <sstream>
#include "parsec-dso.h"
#include "ACommandStringArg.h"
#include "../TierList.h"
#include "../Modules/Tournament.h"

class CommandKOTH : public ACommandStringArg
{
public:
	const COMMAND_TYPE type() override { return COMMAND_TYPE::BOT_MESSAGE; }

	CommandKOTH(const char* msg, TierList& tierList, Tournament& tournament)
		: ACommandStringArg(msg, internalPrefixes()), _tierList(tierList), _tournament(tournament)
	{}

	bool run() override {

		return true;

	}

	static vector<const char*> prefixes() {
		return vector<const char*> { "!koth" };
	}

protected:
	TierList& _tierList;
	Tournament& _tournament;

	static vector<const char*> internalPrefixes() {
		return vector<const char*> { "!koth " };
	}
};