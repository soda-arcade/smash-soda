#pragma once

#include <vector>
#include <sstream>
#include "parsec.h"
#include "Guest.h"
#include "Helpers/Stringer.h"
#include <map>
#include <algorithm>

using namespace std;

#define GUESTLIST_MAX_GUESTS 64

typedef struct MyMetrics
{
	ParsecMetrics metrics{ 0, 0, 0, 0, 0.0, 0.0, 0.0, 0.0 };
	int congested{ 0 };
	float averageNetworkLatency{ 0 };
	uint32_t averageNetworkLatencySize{ 0 };
} MyMetrics;

class GuestList
{
public:
	void setGuests(ParsecGuest* guests, int guestCount);
	vector<Guest> &getGuests();
	vector<Guest> &getPlayingGuests();
	vector<Guest> &getRandomGuests(int count = 1, bool ignoreSpectators = true);
	vector<Guest> &getGuestsAfterGuest(uint32_t targetGuestID, int count = 1, bool ignoreSpectators = true);
	
	void toggleSpectator(uint32_t targetGuestID);

	void clear();
	const bool find(uint32_t targetGuestID, Guest *result);
	const bool find(const char* targetName, Guest* result);
	const bool find(string targetName, Guest* result);
	const int findIndex(uint32_t targetGuestID);
	MyMetrics getMetrics(uint32_t id);
	void deleteMetrics(uint32_t id);
	void updateMetrics(ParsecGuest* guests, int guestCount);

	const char* guestNames[GUESTLIST_MAX_GUESTS];
private:
	vector<Guest> _guests;
	map<uint32_t, MyMetrics> _metrics;
};

