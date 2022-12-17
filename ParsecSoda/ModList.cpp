#include "ModList.h"
#include "Helpers/Stringer.h"

ModList::ModList() {}

ModList::ModList(std::vector<GuestData> moddedUsers)
	: GuestDataList(moddedUsers)
{
}

bool ModList::mod(GuestData user)
{
	bool added = GuestDataList::add(user);
	if (added)
	{
		MetadataCache::saveModdedUsers(_guests);
	}
	return added;
}

const bool ModList::unmod(const uint32_t userID, function<void(GuestData&)> callback)
{
	bool found = GuestDataList::pop(userID, callback);
	if (found)
	{
		MetadataCache::saveModdedUsers(_guests);
	}
	return found;
}

const bool ModList::unmod(string guestName, function<void(GuestData&)> callback)
{
	bool found = GuestDataList::pop(guestName, callback);
	if (found)
	{
		MetadataCache::saveModdedUsers(_guests);
	}
	return found;
}

const bool ModList::isModded(const uint32_t userID)
{
	return find(userID);
}

vector<GuestData>& ModList::getGuests()
{
	return GuestDataList::getGuests();
}