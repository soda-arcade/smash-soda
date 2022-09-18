#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include "GuestData.h"
#include "GuestDataList.h"
#include "MetadataCache.h"

class SpectateList : GuestDataList {

public:
	SpectateList();
	SpectateList(const std::vector<GuestData> spectateUsers);

	bool spectate(const GuestData user);
	const bool unspectate(const uint32_t userID, function<void(GuestData&)> callback);
	const bool unspectate(string guestName, function<void(GuestData&)> callback);
	const bool isSpectating(const uint32_t userID);
	vector<GuestData>& getGuests();
};