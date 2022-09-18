#include "SpectateList.h"
#include "Stringer.h"

SpectateList::SpectateList() {}

SpectateList::SpectateList(std::vector<GuestData> spectateUsers)
	: GuestDataList(spectateUsers)
{
}

bool SpectateList::spectate(GuestData user)
{
	bool added = GuestDataList::add(user);
	return added;
}

const bool SpectateList::unspectate(const uint32_t userID, function<void(GuestData&)> callback)
{
	bool found = GuestDataList::pop(userID, callback);
	return found;
}

const bool SpectateList::unspectate(string guestName, function<void(GuestData&)> callback)
{
	bool found = GuestDataList::pop(guestName, callback);
	return found;
}

const bool SpectateList::isSpectating(const uint32_t userID) {
	return find(userID);
}

vector<GuestData>& SpectateList::getGuests()
{
	return GuestDataList::getGuests();
}