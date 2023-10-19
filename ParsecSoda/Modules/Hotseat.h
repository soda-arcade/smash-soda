#pragma once

#include <vector>
#include <thread>

#include "../Helpers/Stopwatch.h"

//#include "../Models/Seat.h"
#include "../Models/GuestData.h"

/// <summary>
/// Handles the hotseat system.
/// </summary>
class Hotseat {
public:

	class Seat {
	public:
		GuestData guest = GuestData();
		Stopwatch timer;
		Stopwatch reminder;
		int reminderIntervals = 4;
		
		Seat() {
			timer.setDuration(MetadataCache::preferences.hotseatTime * 60000);
			
			// Reminder duration is a 1/4 of timer duration
			uint32_t reminderDuration = (timer.getDuration() / 4);
			reminder.setDuration(reminderDuration);
		}

		void extendTime(int minutes) {
			uint32_t current = MetadataCache::hotseat.hotseatClock.getDuration();
			timer.setDuration(current + (minutes * 60000));

			// Reminder duration is a 1/4 of timer duration
			reminderIntervals = 4;
			uint32_t reminderDuration = (timer.getDuration() / 4);
			reminder.setDuration(reminderDuration);
		}
		
		void decreaseTime(int minutes) {
			uint32_t current = MetadataCache::hotseat.hotseatClock.getDuration();
			timer.setDuration(current - (minutes * 60000));

			// Reminder duration is a 1/4 of timer duration
			reminderIntervals = 4;
			uint32_t reminderDuration = (timer.getDuration() / 4);
			reminder.setDuration(reminderDuration);
		}
	};
	
	bool isStarted = false;
	bool isRunning = false;
	int activeIndex = 0;
	
	vector<Seat> seats;
	vector<GuestData> queue;
	
	Hotseat();
	
	void start();
	void stop();
	
	void enqueue(GuestData guest);
	void dequeue(GuestData guest);
	
	bool unseat(int seatIndex);
	void seatGuest(GuestData guestData, int seatIndex = -1);
	bool unseatGuest(uint32_t guestId);
	bool inSeat(uint32_t guestId);
	
	void extendTime(int minutes, int seatIndex = -1);
	void decreaseTime(int minutes, int seatIndex = -1);

	string formatDuration(long ms);

private:

	thread _startThread;
	thread _runThread;

	void _run();
	
};