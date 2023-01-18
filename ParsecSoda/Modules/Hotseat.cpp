#include "Hotseat.h"

/// <summary>
/// Constructor
/// </summary>
Hotseat::Hotseat() : _parsec(nullptr), _guestList(nullptr), _gamepadClient(nullptr), _chatLog(nullptr) {}

/// <summary>
/// Initialization
/// </summary>
/// <param name="gamepadClient"></param>
void Hotseat::init(ParsecDSO& parsec, GuestList& guestList, GamepadClient& gamepadClient, ChatLog& chatLog) {
	_parsec = &parsec;
	_guestList = &guestList;
	_gamepadClient = &gamepadClient;
	_chatLog = &chatLog;
}

/// <summary>
/// Handles the hotseat swapping
/// </summary>
void Hotseat::run() {

	// Failed tests, abort
	if (!canRun())
		return;

	// Is the active guest still here?
	if (!_activeGuest.isValid() ||
		!_gamepadClient->getGamepad(0)->isOwned() ||
		_hotseatTimer.isFinished() ||
		MetadataCache::isSpectating(_activeGuest)) {

		// Find next hotseat guest
		int index = getActiveIndex();

		// Find next guest
		int nextIndex = index++;
		if (index > _guestList->getGuests().size() - 1)
			index = 0;

		// Is next guest valid?
		Guest guest = _guestList->getGuests()[nextIndex];
		if (guest.isValid() && !MetadataCache::isSpectating(guest)) {

			// Set new guest
			_activeGuest = guest;
			setActive();

		}

	}
	else {

		// Remaining time
		string time = formatDuration(_hotseatTimer.getRemainingTime());

		// Reminder in chat
		if (_reminderTimer.isFinished()) {
			broadcastMessage(_activeGuest.name + " has " + time + " left to play.");
			_reminderTimer.reset();
		}

		// 10 second warning
		if (isReminding && _hotseatTimer.getRemainingTime() < 10000) {
			broadcastMessage("The gamepad is about to be swapped in 10 SECONDS!");
			isReminding = false;
		}


		// Strip pad if AFK
		/*
		if (MetadataCache::preferences.hotseatAFK) {

			ParsecGuest inputGuest;
			ParsecMessage inputGuestMsg;
			if (ParsecHostPollInput(_parsec, 4, &inputGuest, &inputGuestMsg)) {
				_afkTimer.reset(MetadataCache::preferences.hotseatAFKTime);
				_afkTimer.start();
			}

			if (_afkTimer.isFinished()) {
				MetadataCache::removeActiveGuest(_activeGuest);
			}

		}
		*/

	}

}

/// <summary>
/// Checks to see if hotseat mode can be run.
/// Messing with gamepads is BSOD territory, so here we want
/// to make absolutely sure we can go in to hotseat mode.
/// </summary>
/// <returns>boolean</returns>
bool Hotseat::canRun() {

	// Hotseat mode enabled?
	if (!MetadataCache::preferences.hotseat)
		return false;

	// Are there people in the room?
	if (_guestList->getGuests().size() < 1) {
		if (errorCode != 1) {
			_chatLog->logMessage("[HOTSEAT] There needs to be at least one guest in the room.");
			errorCode = 1;
		}
		_gamepadClient->clearOwner(0);
		return false;
	}

	// Ensure at least one gamepad has been enabled
	if (_gamepadClient->gamepads.size() < 1) {
		if (errorCode != 1) {
			_chatLog->logMessage("[HOTSEAT] Please enable at least one Xbox gamepad.");
			errorCode = 1;
		}
		_gamepadClient->clearOwner(0);
		return false;
	}

	// Ensure gamepad 1 is attached and connected
	if (!_gamepadClient->getGamepad(0)->isAttached()) {
		if (errorCode != 2) {
			_chatLog->logMessage("[HOTSEAT] Please enable the first gamepad.");
			errorCode = 2;
		}
		_gamepadClient->clearOwner(0);
		return false;
	}

	// Xbox controller only
	if (_gamepadClient->getGamepad(0)->type() != AGamepad::Type::XBOX) {
		if (errorCode != 3) {
			_chatLog->logMessage("[HOTSEAT] Make sure the first gamepad is an Xbox gamepad (not Playstation controller).");
			errorCode = 3;
		}
		_gamepadClient->clearOwner(0);
		return false;
	}

	// Xbox index 1?
	if (_gamepadClient->getGamepad(0)->getIndex() != 0) {
		if (errorCode != 4) {
			_chatLog->logMessage("[HOTSEAT] Most single player games will only work with the first Xbox controller connected. The host needs to disconnect their controller.");
			errorCode = 4;
		}
		_gamepadClient->clearOwner(0);
		return false;
	}

	// Is everybody spectating?
	if (MetadataCache::preferences.activeGuests.size() < 1) {
		if (errorCode != 5) {
			_chatLog->logMessage("[HOTSEAT] Everybody is spectating!");
			errorCode = 5;
		}
		_gamepadClient->clearOwner(0);
		return false;
	}

	errorCode = -1;
	return true;

}

/// <summary>
/// Sets the current hotseat guest.
/// </summary>
void Hotseat::setActive() {

	// Give pad to hotseat guest
	_gamepadClient->getGamepad(0)->setOwner(_activeGuest, 0, false);

	// Set timers
	uint32_t minutes = MetadataCache::preferences.hotseatTime * 60000;

	_hotseatTimer.reset(minutes);
	_reminderTimer.reset(minutes / 4);
	_afkTimer.reset(MetadataCache::preferences.hotseatAFKTime * 60000);

	_hotseatTimer.start();
	_reminderTimer.start();
	_afkTimer.start();

	// Chatbot
	broadcastMessage(_activeGuest.name + " now has control of the gamepad!");
	isReminding = true;

}

/// <summary>
/// Gets current hotseat guest's index in guest list.
/// </summary>
/// <returns>index</returns>
int Hotseat::getActiveIndex() {

	// Loop through guest list until we find current hotseat guest
	if (_guestList->getGuests().size() > 0) {
		for (int i = 0; i < _guestList->getGuests().size(); i++) {
			if (_guestList->getGuests()[i].userID == _activeGuest.userID)
				return i;
		}
	}

	// Hotseat guest not valid or not chosen yet, put first guest in hotseat
	return 0;

}

/// <summary>
/// Formats a status message.
/// </summary>
/// <param name="message">The status message</param>
/// <returns>Formatted message</returns>
void Hotseat::broadcastMessage(string message) {

	vector<Guest> guests = _guestList->getGuests();
	vector<Guest>::iterator gi;

	string msg = "[HOTSEAT] " + message;
	_chatLog->logMessage(msg);

	for (gi = guests.begin(); gi != guests.end(); ++gi) {
		ParsecHostSendUserData(_parsec, (*gi).id, 0, msg.c_str());
	}

}

/// <summary>
/// Helper function for formatting duration.
/// </summary>
/// <param name="ms">Duration in milliseconds</param>
/// <returns>string</returns>
string Hotseat::formatDuration(long ms) {

	//3600000 milliseconds in an hour
	long hr = ms / 3600000;
	ms = ms - 3600000 * hr;

	//60000 milliseconds in a minute
	long min = ms / 60000;
	ms = ms - 60000 * min;

	//1000 milliseconds in a second
	long sec = ms / 1000;
	ms = ms - 1000 * sec;

	ostringstream output;
	output << min << "min " << sec << "sec";

	return output.str();

}