#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include "matoya.h"
#include "../Models/GuestData.h"
#include "../GuestDataList.h"
#include "../Helpers/PathHelper.h"

class BanList : GuestDataList
{
public:
	BanList();
	BanList(const std::vector<GuestData> bannedUsers);
	bool ban (const GuestData user);
	const bool unban(const uint32_t userID, function<void(GuestData&)> callback);
	const bool unban(string guestName, function<void(GuestData&)> callback);
	const bool isBanned(const uint32_t userID);
	vector<GuestData>& getGuests();
	static vector<GuestData> LoadFromFile();
	bool SaveToFile();
};

