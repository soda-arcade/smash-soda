#include "CommandBB.h"

void CommandBB::init() {

}

void CommandBB::pressB() {

	ParsecGamepadButtonMessage button = {};
	button.button = ParsecGamepadButton::GAMEPAD_BUTTON_B;
	button.pressed = true;
	CommandBB::pressButton(button);

}

void CommandBB::releaseB() {

	ParsecGamepadButtonMessage button = {};
	button.button = ParsecGamepadButton::GAMEPAD_BUTTON_B;
	button.pressed = false;
	CommandBB::pressButton(button);

}

void CommandBB::pressButton(ParsecGamepadButtonMessage button) {

	ParsecMessage message = {};
	message.type = ParsecMessageType::MESSAGE_GAMEPAD_BUTTON;
	message.gamepadButton = button;

	// For every gamepad
	std::vector<AGamepad*>::iterator gi = _gamepadClient.gamepads.begin();
	int onlineCount = 0;
	for (; gi != _gamepadClient.gamepads.end(); ++gi) {
		if ((*gi)->isConnected())
			_gamepadClient.sendMessage((*gi)->owner.guest, message);
	}

}