#pragma once

#include "../GamepadClient.h"
#include "../Helpers/Stopwatch.h"
#include "../ChatLog.h"

/// <summary>
/// Handles the hotseat system.
/// </summary>
class Hotseat {
public:
	Hotseat();
	void init(ParsecDSO& parsec, GuestList& guestList, GamepadClient& gamepadClient, ChatLog& chatLog);

	void run();
private:
	ParsecDSO* _parsec;
	GuestList* _guestList;
	GamepadClient* _gamepadClient;
	ChatLog* _chatLog;

	Guest _activeGuest;
	Guest _nextGuest;

	Stopwatch _hotseatTimer;
	Stopwatch _reminderTimer;
	Stopwatch _afkTimer;

	int errorCode = -1;
	bool isReminding = false;

	void broadcastMessage(string message);
	string formatDuration(long ms);

	bool canRun();
	void setActive();
	int getActiveIndex();

};