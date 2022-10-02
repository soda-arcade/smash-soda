#pragma once

#include "ACommand.h"
#include <iostream>
#include "../GuestList.h"
#include <sstream>

class CommandPingAll : public ACommand
{
public:

	const COMMAND_TYPE type() override { return COMMAND_TYPE::PADS; }

	CommandPingAll(GuestList& guestList)
		: _guestList(guestList)
	{}

	bool run() override
	{
		std::ostringstream reply;
		reply << MetadataCache::preferences.chatbotName + "\n";

		for (int i = 0; i < _guestList.getGuests().size(); i++) {

			ParsecMetrics metrics = _guestList.getMetrics(_guestList.getGuests()[i].id).metrics;
			stringstream ping;
			ping << trunc(metrics.networkLatency);

			reply << _guestList.getGuests()[i].name << ": [" << ping.str() << "ms]\n";

		}

		reply << "\0";
		_replyMessage = reply.str();

		return true;
	}

	static vector<const char*> prefixes()
	{
		return vector<const char*> { "!pingall" };
	}

protected:
	GuestList& _guestList;
};
