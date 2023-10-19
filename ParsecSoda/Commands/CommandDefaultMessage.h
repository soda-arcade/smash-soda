#pragma once

#include <iostream>
#include <sstream>
#include "ACommand.h"
#include "../Guest.h"
#include "../Tier.h"
#include "../Helpers/Stringer.h"
#include "../VIPList.h"

class CommandDefaultMessage : public ACommand
{
public:
	const COMMAND_TYPE type() override { return COMMAND_TYPE::DEFAULT_MESSAGE; }

	CommandDefaultMessage(const char* msg, Guest &sender, uint32_t lastUserID, Tier tier, VIPList& vipList, bool isHost = false)
		: _msg(msg), _sender(sender), _lastUserID(lastUserID), _tier(tier), _vipList(vipList), _isHost(isHost)
	{}

	bool run() override
	{
		std::ostringstream reply;
		if (_sender.userID != _lastUserID)
		{
			static string role = "";
			if (_isHost || _tier == Tier::GOD) role = "[H]  ";
			else if (_tier == Tier::ADMIN || _tier == Tier::MOD) role = "[M]  ";
			else if (_vipList.isVIP(_sender.userID)) role = "[V]  ";
			else role = ">  ";
			
			if (_sender.isValid())
			{
				reply << role << _sender.name << " \t (#" << _sender.userID << ")";
			}
			else if(_isHost)
			{
				reply << role << "Host";
			}
			else
			{
				reply << role << "Unkown Guest";
			}

			reply << ":\n";
		}

		reply << "\t\t " << _msg << "\0";

		_replyMessage = reply.str();
		reply.clear();
		reply.flush();

		return true;
	}

protected:
	string _msg;
	Guest &_sender;
	uint32_t _lastUserID;
	Tier _tier;
	VIPList& _vipList;
	bool _isHost;
};