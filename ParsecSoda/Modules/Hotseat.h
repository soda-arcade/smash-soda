#pragma once
#include "../GamepadClient.h"
#include "../Helpers/Stopwatch.h"

/// <summary>
/// Handles the hotseat system.
/// </summary>
class Hotseat {
public:
	Hotseat();
	void init(GuestList& guestList, GamepadClient& gamepadClient);

	void run();
private:
	GuestList* _guestList;
	GamepadClient* _gamepadClient;

	Guest _currentGuest;
	Guest _nextGuest;

	Stopwatch _hotseatTimer;
	Stopwatch _reminderTimer;

	bool test();
	void setCurrent();
	void findCurrent();
	void startTimer();
	void stopTimer();

	string msg(string message);
};