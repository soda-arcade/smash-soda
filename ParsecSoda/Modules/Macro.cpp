#include "Macro.h"

/// <summary>
/// Constructor
/// </summary>
Macro::Macro()
    : _gamepadClient(nullptr), _masterOfPuppets(nullptr), masterIndex(-1)
{
}

/// <summary>
/// Initializes Macro module.
/// </summary>
/// <param name="gamepadClient"></param>
void Macro::init(GamepadClient& gamepadClient, MasterOfPuppets& masterOfPuppets) {
    _gamepadClient = &gamepadClient;
	_masterOfPuppets = &masterOfPuppets;
}

/// <summary>
/// Runs any enqueued macro commands.
/// </summary>
void Macro::run() {

	// Buttons in queue?
	if (cmdList.size() > 0) {

		// Press in
		if (!cmdList.front().pressed) {

			ParsecGamepadButtonMessage btn = createButtonMessage(cmdList.front().button, true);
			sendButtonForAll(btn);
			cmdList.front().pressed = true;

		}
		else {

			// Depress
			ParsecGamepadButtonMessage btn = createButtonMessage(cmdList.front().button, false);
			sendButtonForAll(btn);

			// Remove from queue
			cmdList.erase(cmdList.begin());

		}

	}

	// Waiting for button queue to clear?
	if (isWaiting && waitTimer.isFinished()) {

		// Disable puppets
		_masterOfPuppets->setMasterIndex(-1);
		std::vector<AGamepad*>::iterator gi = _gamepadClient->gamepads.begin() + 1;
		for (; gi != _gamepadClient->gamepads.end(); ++gi) {
			(*gi)->isPuppet = false;
		}

		isWaiting = false;

	}

}

/// <summary>
/// Enqueues a button press for everyone.
/// </summary>
/// <param name="button"></param>
void Macro::pressButtonForAll(ParsecGamepadButton button) {

	// Start a stopwatch to disable puppets
	waitTimer.reset(1000 * cmdList.size());
	waitTimer.start();
	isWaiting = true;

	// Create new command
	Macro::Cmd cmd = Macro::Cmd();
	cmd.button = button;

	// Enqueue
	cmdList.push_back(cmd);

}

/// <summary>
/// Press a button for ALL connected guests in your room.
/// </summary>
/// <param name="button">The button to press.</param>
void Macro::sendButtonForAll(ParsecGamepadButtonMessage button) {

	ParsecMessage message = {};
	message.type = ParsecMessageType::MESSAGE_GAMEPAD_BUTTON;
	message.gamepadButton = button;

	// Mark first pad as master
	_masterOfPuppets->setMasterIndex(0);

	// Mark other gamepads as puppets
	std::vector<AGamepad*>::iterator gi = _gamepadClient->gamepads.begin() + 1;
	for (; gi != _gamepadClient->gamepads.end(); ++gi) {
		(*gi)->isPuppet = true;
	}

	// Master puppet presses button for all
	_gamepadClient->sendMessage(_gamepadClient->gamepads[0]->owner.guest, message);

}

/// <summary>
/// Creates a new button gamepad message.
/// </summary>
/// <param name="button">The button to press.</param>
/// <param name="in">Is the button pressed in or not.</param>
/// <returns>ParsecGamepadButtonMessage</returns>
ParsecGamepadButtonMessage Macro::createButtonMessage(ParsecGamepadButton button, bool in) {

	ParsecGamepadButtonMessage msg = {};
	msg.button = button;
	msg.pressed = in;

	return msg;

}