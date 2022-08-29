#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include "GuestData.h"
#include "GuestDataList.h"
#include "MetadataCache.h"

class ModList : GuestDataList {

public:
	ModList();
	ModList(const std::vector<GuestData> moddedUsers);

	bool mod(const GuestData user);
	const bool unmod(const uint32_t userID, function<void(GuestData&)> callback);
	const bool unmod(string guestName, function<void(GuestData&)> callback);
	const bool isModded(const uint32_t userID);
	vector<GuestData>& getGuests();

};