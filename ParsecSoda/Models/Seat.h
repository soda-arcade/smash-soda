#pragma once

#include "GuestData.h"
#include "../Helpers/Stopwatch.h"

class Seat {
public:
	bool expires = false;
	GuestData guest = GuestData();
	Stopwatch timer;
	
	Seat(bool expires);
	void extendTime(int minutes);
	void decreaseTime(int minutes);
};