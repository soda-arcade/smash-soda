#include "../Hosting.h"
extern Hosting g_hosting;
#include "Hotseat.h"

/// <summary>
/// Constructor
/// </summary>
Hotseat::Hotseat() {}

/// <summary>
/// Start the hotseat system.
/// </summary>
void Hotseat::start() {

	_startThread = thread([&]() {

		// Get gamepad client
		GamepadClient& gamepadClient = g_hosting.getGamepadClient();

		// Is another system already running? Abort
		if (gamepadClient.isBusy() || gamepadClient.isSlave) {
			return;
		}

		// Take control of gamepads
		gamepadClient.isSlave = true;

		// Get number of hotseats
		int seatCount = MetadataCache::preferences.hotseatSeats;

		// Create seats
		for (int i = 0; i < seatCount; i++) {
			Seat seat = Seat();
			seats.push_back(seat);
		}

		// Create gamepads for seats
		int xboxPads = seatCount < 4 ? seatCount : 4;
		int ds4Pads = seatCount > 4 ? seatCount - 4 : 0;

		// Clear existing pads
		gamepadClient.releaseGamepads();
		Sleep(200);
		
		for (uint16_t i = 0; i < xboxPads; i++) {
			gamepadClient.createGamepad(AGamepad::Type::XBOX);
			Sleep(200);
		}
		for (uint16_t i = 0; i < ds4Pads; i++) {
			gamepadClient.createGamepad(AGamepad::Type::DUALSHOCK);
			Sleep(200);
		}
		std::vector<AGamepad*>::iterator gi = gamepadClient.gamepads.begin();
		for (; gi != gamepadClient.gamepads.end(); ++gi) {
			(*gi)->connect();
			Sleep(200);
		}
		gi = gamepadClient.gamepads.begin();
		for (; gi != gamepadClient.gamepads.end(); ++gi) {
			(*gi)->refreshIndex();
		}
		Sleep(200);

		// Get guest list
		vector<Guest>& guests = g_hosting.getGuests();
		for (uint16_t i = 0; i < guests.size(); i++) {
			GuestData guestData = GuestData(guests[i].name, guests[i].userID);
			enqueue(guestData);
		}
		Sleep(200);

		// Start the run thread
		_runThread = thread([&]() {
			_run();
		});
		_runThread.detach();

	});
	
	_startThread.detach();

}

/// <summary>
/// Stop the hotseat system.
/// </summary>
void Hotseat::stop() {

	// Stop thread
	isRunning = false;
	isStarted = false;

	// Get gamepad client
	GamepadClient& gamepadClient = g_hosting.getGamepadClient();
	if (gamepadClient.isBusy()) {
		return;
	}

	// Clear queue and seats
	queue.clear();
	seats.clear();

	// Release gamepads
	gamepadClient.isSlave = false;
	gamepadClient.releaseGamepads();
	
}

/// <summary>
/// Adds a guest to the queue.
/// </summary>
/// <param name="guest"></param>
void Hotseat::enqueue(GuestData guest) {

	if (!MetadataCache::isSpectating(guest.userID)) {
		
		// For each seat
		for (int i = 0; i < seats.size(); i++) {

			// Get seat
			Seat& seat = seats[i];

			// If seat is empty
			if (seat.guest.userID == 0) {

				// Put the guest in the seat
				seatGuest(guest, i);
				break;

			}

		}
		
		queue.push_back(guest);
	}

}

/// <summary>
/// Removes a guest from the queue.
/// </summary>
/// <param name="guest"></param>
void Hotseat::dequeue(GuestData guest) {

	// Remove from queue
	for (int i = 0; i < queue.size(); i++) {
		if (queue[i].userID == guest.userID) {
			queue.erase(queue.begin() + i);
			return;
		}
	}

	// Unseat guest if in hotseat
	for (int i = 0; i < seats.size(); i++) {
		Seat& seat = seats[i];
		if (seat.guest.userID == guest.userID) {
			unseat(i);
			return;
		}
	}

}

/// <summary>
/// Extends time on the seat.
/// </summary>
/// <param name="minutes"></param>
/// <param name="seatIndex">optional</param>
void Hotseat::extendTime(int minutes, int seatIndex) {

	// Everyone
	if (seatIndex == -1) {

		// For each seat
		for (int i = 0; i < seats.size(); i++) {

			// Get seat
			Seat& seat = seats[i];

			// Extend time
			int currentTime = seat.timer.getDuration();
			seat.timer.setDuration(currentTime + (minutes * 60000));

			// Update reminder
			seat.reminderIntervals = 4;
			uint32_t reminderDuration = (seat.timer.getDuration() / 4);
			seat.reminder.setDuration(reminderDuration);

		}
		
	}
	else {

		// Get seat
		Seat& seat = seats[seatIndex];

		// Extend time
		int currentTime = seat.timer.getDuration();
		seat.timer.setDuration(currentTime + (minutes * 60000));

		// Update reminder
		seat.reminderIntervals = 4;
		uint32_t reminderDuration = (seat.timer.getDuration() / 4);
		seat.reminder.setDuration(reminderDuration);
		
	}

}

/// <summary>
/// Decrease time on the seat.
/// </summary>
/// <param name="minutes"></param>
/// <param name="seatIndex">optional</param>
void Hotseat::decreaseTime(int minutes, int seatIndex) {

	// Everyone
	if (seatIndex == -1) {

		// For each seat
		for (int i = 0; i < seats.size(); i++) {

			// Get seat
			Seat& seat = seats[i];

			// Time can't be below zero
			int currentTime = seat.timer.getDuration();
			int newTime = currentTime - (minutes * 60000);
			if (newTime < 0) {
				newTime = 0;
			}

			// Extend time
			seat.timer.setDuration(newTime);

			// Update reminder
			seat.reminderIntervals = 4;
			uint32_t reminderDuration = (seat.timer.getDuration() / 4);
			seat.reminder.setDuration(reminderDuration);

		}

	}
	else {

		// Get seat
		Seat& seat = seats[seatIndex];

		// Time can't be below zero
		int currentTime = seat.timer.getDuration();
		int newTime = currentTime - (minutes * 60000);
		if (newTime < 0) {
			newTime = 0;
		}

		// Extend time
		seat.timer.setDuration(newTime);

		// Update reminder
		seat.reminderIntervals = 4;
		uint32_t reminderDuration = (seat.timer.getDuration() / 4);
		seat.reminder.setDuration(reminderDuration);

	}
	
}

/// <summary>
/// Hotseat loop every 1 second 
/// </summary>
void Hotseat::_run() {

	// Start all seat timers at same time
	for (int i = 0; i < seats.size(); i++) {
		seats[i].timer.start();
		seats[i].reminder.start();
		seats[i].reminderIntervals = 4;
	}

	// Start
	isRunning = true;

	// Loop
	while (isRunning) {

		// For each seat
		for (int i = 0; i < seats.size(); i++) {

			// Get seat
			Seat& seat = seats[i];
			if (seat.guest.userID == 0 ||
				g_hosting.getGuests().size() < seats.size()) {
				seats[i].timer.reset();
				seats[i].reminder.reset();
				seats[i].reminderIntervals = 4;
				continue;
			}

			// If seat is expired
			if (seat.timer.isFinished()) {
				
				seat.timer.reset();

				if (g_hosting.getGuests().size() >= seats.size()) {
					unseat(i);
				}

			}

			// If time for next reminder
			if (seat.reminder.isFinished() && seat.reminderIntervals > 0) {

				// Get guest
				Guest& guest = g_hosting.getGuests()[g_hosting.getGuestIndex(seat.guest.userID)];

				// Get remaining time
				int remaining = seat.timer.getRemainingTime();
				
				// Send reminder
				string message = guest.name + " has " + formatDuration(remaining) + " left in the hotseat.";
				g_hosting.logMessage(message);
				
				// Reset timer
				seat.reminder.reset();
				seat.reminderIntervals--;

			}

		}

		// Sleep for 1 second
		Sleep(1000);
		
	}
	
}

/// <summary>
/// Removes a guest from a seat.
/// </summary>
/// <param name="seatIndex"></param>
/// <returns></returns>
bool Hotseat::unseat(int seatIndex) {

	// Get seat
	Seat& seat = seats[seatIndex];
	GuestData currentGuest = seat.guest;

	// Reset seat
	seat.guest = GuestData();
	
	// Erase guest from queue
	int guestIndex = 0;
	for (int i = 0; i < queue.size(); i++) {
		if (queue[i].userID == currentGuest.userID) {
			queue.erase(queue.begin() + i);
			guestIndex = i;
			break;
		}
	}
	
	// Add guest to end of queue
	queue.push_back(currentGuest);

	// Go through queue and find new guest to put in seat
	for (int i = guestIndex; i < queue.size(); i++) {

		// If guest invalid?
		bool found = false;
		vector<Guest>& guestList = g_hosting.getGuests();
		for (int j = 0; j < guestList.size(); j++) {
			if (guestList[j].userID == queue[i].userID) {
				found = true;
			}
		}
		if (!found || MetadataCache::isSpectating(queue[i].userID)) {
			dequeue(queue[i]);
			continue;
		}

		// Get guest
		if (!inSeat(queue[i].userID)) {
			
			// Put guest in seat
			seat.guest.name = queue[i].name;
			seat.guest.userID = queue[i].userID;

			// Get guest
			int guestIndex = g_hosting.getGuestIndex(queue[i].userID);
			if (guestIndex != -1) {

				// Get guest
				Guest& guest = g_hosting.getGuests()[guestIndex];

				// Assign pad
				g_hosting.getGamepadClient().getGamepad(seatIndex)->setOwner(guest, 0, false);

				string message = queue[i].name + " has been put in seat " + to_string(seatIndex + 1) + ".";
				g_hosting.logMessage(message);

			}
			
			break;
		}

	}

	// Reset timer
	seat.timer.reset();
	seat.reminder.reset();
	seat.reminderIntervals = 4;

	return true;

}

/// <summary>
/// Checks to see if a guest is already in a seat.
/// </summary>
/// <param name="guestID"></param>
/// <returns></returns>
bool Hotseat::inSeat(uint32_t guestID) {

	// Is guest already in a seat?
	for (int i = 0; i < seats.size(); i++) {
		if (seats[i].guest.userID == guestID) {
			return true;
		}
	}

	return false;
	
}

/// <summary>
/// Puts a guest in a seat.
/// </summary>
/// <param name="guestID"></param>
/// <returns></returns>
void Hotseat::seatGuest(GuestData guestData, int seatIndex) {

	if (seatIndex == -1) {

		// Find first available seat
		for (int i = 0; i < seats.size(); i++) {
			if (seats[i].guest.userID == 0) {
				seatIndex = i;
				break;
			}
		}
		
	}
	
	// Get seat
	Seat& seat = seats[seatIndex];
	seat.timer.reset();
	seat.reminder.reset();
	seat.reminderIntervals = 4;
	
	// Get guest
	int guestIndex = g_hosting.getGuestIndex(guestData.userID);
	if (guestIndex != -1) {
		
		// Get guest
		Guest& guest = g_hosting.getGuests()[guestIndex];

		// Assign pad
		g_hosting.getGamepadClient().getGamepad(seatIndex)->setOwner(guest, 0, false);
		
		g_hosting.logMessage(guestData.name + " has been put in seat " + to_string(seatIndex + 1) + ".");
	}
	else {
		
	}

}

/// <summary>
/// Forcibly removes a guest from a seat.
/// </summary>
/// <param name="guestID"></param>
/// <returns></returns>
bool Hotseat::unseatGuest(uint32_t guestID) {

	// Is guest already in a seat?
	for (int i = 0; i < seats.size(); i++) {
		if (seats[i].guest.userID == guestID) {
			unseat(i);
			return true;
		}
	}

	return false;

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