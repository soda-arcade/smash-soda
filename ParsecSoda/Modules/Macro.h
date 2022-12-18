#pragma once
#include "../GamepadClient.h"
#include "../MasterOfPuppets.h"
#include "../Helpers/Stopwatch.h"

/// <summary>
/// Does all the automatic button pressing stuff.
/// </summary>
class Macro {

public:
	Macro();
	void init(GamepadClient& gamepadClient, MasterOfPuppets& masterOfPuppets);

	void run();
	void pressButtonForAll(ParsecGamepadButton button);
	//void pressButtonForPad(int padIndex, ParsecGamepadButton button);
	//void pressButtonForGuest(Guest guest, ParsecGamepadButton button);

private:

	class Cmd {
	public:
		int padIndex = -1;
		ParsecGamepadButton button;
		bool pressed = false;
	};

	int masterIndex;
	GamepadClient* _gamepadClient;
	MasterOfPuppets* _masterOfPuppets;

	vector<Cmd> cmdList;

	bool isWaiting = false;
	Stopwatch waitTimer;

	void sendButtonForAll(ParsecGamepadButtonMessage button);
	ParsecGamepadButtonMessage createButtonMessage(ParsecGamepadButton button, bool in);
	//void sendButtonForPad(int padIndex, ParsecGamepadButtonMessage button);

};