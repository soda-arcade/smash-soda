#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include "Models/GuestData.h"
#include "GuestDataList.h"
#include "MetadataCache.h"

class VIPList : GuestDataList
{
public:
	VIPList();
	VIPList(const std::vector<GuestData> vipUsers);
	bool VIP(const GuestData user);
	const bool unVIP(const uint32_t userID, function<void(GuestData&)> callback);
	const bool unVIP(string guestName, function<void(GuestData&)> callback);
	const bool isVIP(const uint32_t userID);
	vector<GuestData>& getGuests();
};