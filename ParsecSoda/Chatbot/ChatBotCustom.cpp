#include "ChatBotCustom.h"

/*
* Include all your custom commands here.
* 
* Changes to this file and files inside Commands/Custom
* folder will be ignored by git, to avoid conflicts when
* pulling updates from the repository.
*/

//#include "Commands/Custom/YourCommandClass.h"

/**
* This function is where you check to see if the
* message matches your command prefixes.
**/
ACommand* ChatBotCustom::isCustomCommand(const char* msg, Guest& sender, bool isHost, Tier tier, uint32_t previous) {

	//if (msgIsEqual(msg, YourCommandClass::prefixes()))	return new YourCommandClass(sender, _gamepadClient, _macro);

	// Returns a default message if no custom command is found, so the bot can still respond.
	return new CommandDefaultMessage(msg, sender, previous, tier, isHost);
}

/**
* Don't edit anything below this line. Use these functions
* to check if a message starts with a pattern or is equal to
* a pattern.
**/

bool ChatBotCustom::msgStartsWith(const char* msg, const char* pattern) {
	return Stringer::startsWithPattern(msg, pattern);
}

bool ChatBotCustom::msgStartsWith(const char* msg, vector<const char*> patterns) {
	vector<const char*>::iterator pi = patterns.begin();
	for (; pi != patterns.end(); ++pi)
	{
		if (Stringer::startsWithPattern(msg, *pi))
		{
			return true;
		}
	}
	return false;
}

bool ChatBotCustom::msgIsEqual(const char* msg, const char* pattern) {
	return (strcmp(msg, pattern) == 0);
}

bool ChatBotCustom::msgIsEqual(const char* msg, vector<const char*> patterns) {
	vector<const char*>::iterator pi = patterns.begin();
	for (; pi != patterns.end(); ++pi)
	{
		if ((strcmp(msg, *pi) == 0))
		{
			return true;
		}
	}
	return false;
}