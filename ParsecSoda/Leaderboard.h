#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include "GuestData.h"
#include "GuestDataList.h"
#include "MetadataCache.h"

class Leaderboard : GuestDataList
{
public:
	Leaderboard();
	Leaderboard(const std::vector<GuestData> leaderboardUsers);
	bool update(const GuestData user);
	const bool remove(const uint32_t userID, function<void(GuestData&)> callback);
	const bool remove(string guestName, function<void(GuestData&)> callback);
	const bool isOnLeaderboard(const uint32_t userID);
	vector<GuestData>& getGuests();
};