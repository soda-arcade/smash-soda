#pragma once

#include "../Base/ACommand.h"

using namespace std;

class CommandTriangle : public ACommand
{
public:

	/**
	 * @brief Construct a new CommandTriangle object
	 * 
	 * @param sender 
	 * @param gamepadClient 
	 * @param macro 
	 */
	CommandTriangle(Guest& sender, GamepadClient& gamepadClient, Macro& macro)
		: _sender(sender), _gamepadClient(gamepadClient), _macro(macro)
	{}

	/**
	 * @brief Run the command
	 * 
	 * @return true 
	 * @return false 
	 */
	bool run() override {

		// Random seed from time
		srand(time(NULL));

		int randomNumber = 1 + (rand() % 10);
		
		SetReply("Triangle button pressed " + to_string(randomNumber) + " times!");

		/*for (int i = 0; i < randomNumber; i++) {
			_macro.pressButtonForGuest(_sender.userID, ParsecGamepadButton::GAMEPAD_BUTTON_Y);
		}*/

		return true;

	}

	/**
	 * @brief Get the prefixes object
	 * 
	 * @return vector<const char*> 
	 */
	static vector<const char*> prefixes() {
		return vector<const char*> { "!triangle" };
	}

protected:
	Guest& _sender;
	GamepadClient& _gamepadClient;
	Macro& _macro;
};