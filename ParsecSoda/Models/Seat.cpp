#include "Seat.h"

/// <summary>
/// Constructor
/// </summary>
/// <param name="expires"></param>
Seat::Seat(bool expires = false) {
	this->expires = expires;
}

/// <summary>
/// Extends time on the seat.
/// </summary>
/// <param name="minutes"></param>
void Seat::extendTime(int minutes) {
	if (!expires) {
		return;
	}
	int currentTime = timer.getDuration();
	timer.setDuration(currentTime + (minutes * 60));
}

/// <summary>
/// Decrease time on the seat.
/// </summary>
/// <param name="minutes"></param>
void Seat::decreaseTime(int minutes) {
	if (!expires) {
		return;
	}
	int currentTime = timer.getDuration();
	timer.setDuration(currentTime - (minutes * 60));
}