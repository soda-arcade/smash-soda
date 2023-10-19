#pragma once

#include <sstream>
#include "parsec-dso.h"
#include "ACommandStringArg.h"
#include "../TierList.h"
#include "../Modules/Tournament.h"

class Command1v1 : public ACommandStringArg
{
public:
	const COMMAND_TYPE type() override { return COMMAND_TYPE::BOT_MESSAGE; }

	Command1v1(const char* msg, TierList& tierList, Tournament& tournament)
		: ACommandStringArg(msg, internalPrefixes()), _tierList(tierList), _tournament(tournament)
	{}

	bool run() override {

		return true;

	}

	static vector<const char*> prefixes() {
		return vector<const char*> { "!1v1" };
	}

protected:
	TierList& _tierList;
	Tournament& _tournament;

	static vector<const char*> internalPrefixes() {
		return vector<const char*> { "!1v1 " };
	}
};