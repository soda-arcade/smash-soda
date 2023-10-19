#pragma once
#include "../GamepadClient.h"
#include "../GuestList.h"
#include "../Helpers/Stopwatch.h"

/// <summary>
/// Does all the automatic button pressing stuff.
/// </summary>
class Macro {

public:
	Macro();
	void init(GamepadClient& gamepadClient, GuestList& guestList);

	void run();
	void pressButtonForAll(ParsecGamepadButton button);
	void pressButtonForPad(int padIndex, ParsecGamepadButton button);
	void pressButtonForGuest(int guestId, ParsecGamepadButton button);

private:

	class Cmd {
	public:
		int padIndex = -1;
		int guestId = -1;
		ParsecGamepadButton button;
		bool pressed = false;
	};

	GamepadClient* _gamepadClient;
	GuestList* _guestList;

	vector<Cmd> cmdList;

	void sendButtonForAll(ParsecGamepadButtonMessage button);
	ParsecGamepadButtonMessage createButtonMessage(ParsecGamepadButton button, bool in);
	void sendButtonForPad(int padIndex, ParsecGamepadButtonMessage button);
	void sendButtonForGuest(int guestId, ParsecGamepadButtonMessage button);

};