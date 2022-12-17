#include "VIPList.h"
#include "Helpers/Stringer.h"

VIPList::VIPList()
{
}

VIPList::VIPList(std::vector<GuestData> vipUsers)
	: GuestDataList(vipUsers)
{
}

bool VIPList::VIP(GuestData user)
{
	bool added = GuestDataList::add(user);
	if (added)
	{
		MetadataCache::saveVIPUsers(_guests);
	}
	return added;
}

const bool VIPList::unVIP(const uint32_t userID, function<void(GuestData&)> callback)
{
	bool found = GuestDataList::pop(userID, callback);
	if (found)
	{
		MetadataCache::saveVIPUsers(_guests);
	}
	return found;
}

const bool VIPList::unVIP(string guestName, function<void(GuestData&)> callback)
{
	bool found = GuestDataList::pop(guestName, callback);
	if (found)
	{
		MetadataCache::saveVIPUsers(_guests);
	}
	return found;
}

const bool VIPList::isVIP(const uint32_t userID)
{
	return find(userID);
}

vector<GuestData>& VIPList::getGuests()
{
	return GuestDataList::getGuests();
}
