#pragma once

#include <vector>
#include <sstream>
#include "parsec.h"
#include "Guest.h"
#include "Stringer.h"
#include <map>

using namespace std;

#define GUESTLIST_MAX_GUESTS 64

typedef struct MyMetrics
{
	ParsecMetrics metrics{ 0, 0, 0, 0, 0.0, 0.0, 0.0, 0.0 };
	int congested{ 0 };
} MyMetrics;

class GuestList
{
public:
	void setGuests(ParsecGuest* guests, int guestCount);
	vector<Guest> &getGuests();
	void clear();
	const bool find(uint32_t targetGuestID, Guest *result);
	const bool find(const char* targetName, Guest* result);
	const bool find(string targetName, Guest* result);
	MyMetrics getMetrics(uint32_t id);
	void deleteMetrics(uint32_t id);
	void updateMetrics(ParsecGuest* guests, int guestCount);

	const char* guestNames[GUESTLIST_MAX_GUESTS];
private:
	vector<Guest> _guests;
	map<uint32_t, MyMetrics> _metrics;
};

