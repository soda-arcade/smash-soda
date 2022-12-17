#pragma once

#include "ACommand.h"
#include <iostream>
#include "../CompilerDirectives.h"
#include "../Helpers/Dice.h"

using namespace std;

class Command8Ball : public ACommand
{
public:
	const COMMAND_TYPE type() override { return COMMAND_TYPE::BOT_MESSAGE; }

	Command8Ball(Guest& sender)
		: _sender(sender)
	{}

	bool run() override {

		srand(time(NULL));

		if (Dice::roll(50)) {
			_replyMessage = "[8BALL] " + _yes[(rand() % 10)] + "\0";
		} 
		else {

			if (Dice::roll(50)) {
				_replyMessage = "[8BALL] " + _maybe[(rand() % 5)] + "\0";
			}
			else {
				_replyMessage = "[8BALL] " + _no[(rand() % 5)] + "\0";
			}

		}

		_replyMessage += ", " + _sender.name + ".";

		return true;

	}

	static vector<const char*> prefixes() {
		return vector<const char*> { "!8ball" };
	}

protected:
	Guest& _sender;

	string _yes[10] = {
		"As I see it, yes",
		"It is certain",
		"It is decidedly so",
		"Most likely",
		"Outlook good",
		"Signs point to yes",
		"Without a doubt",
		"Yes",
		"Yes - definitely",
		"You may rely on it"
	};

	string _maybe[5] = {
		"Reply hazy, try again",
		"Ask again later",
		"Better not tell you now",
		"Cannot predict now",
		"Concentrate and ask again"
	};

	string _no[5] = {
		"Don't count on it",
		"My reply is no",
		"My sources say no",
		"Outlook not so good",
		"Very doubtful"
	};
};