#include "Leaderboard.h"
#include "Stringer.h"

Leaderboard::Leaderboard()
{
}

Leaderboard::Leaderboard(std::vector<GuestData> leaderboardUsers)
	: GuestDataList(leaderboardUsers)
{
}

bool Leaderboard::update(GuestData user)
{
	bool added = GuestDataList::add(user);
	if (added)
	{
		MetadataCache::saveLeaderboardUsers(_guests);
	}
	return added;
}

const bool Leaderboard::remove(const uint32_t userID, function<void(GuestData&)> callback)
{
	bool found = GuestDataList::pop(userID, callback);
	if (found)
	{
		MetadataCache::saveLeaderboardUsers(_guests);
	}
	return found;
}

const bool Leaderboard::remove(string guestName, function<void(GuestData&)> callback)
{
	bool found = GuestDataList::pop(guestName, callback);
	if (found)
	{
		MetadataCache::saveLeaderboardUsers(_guests);
	}
	return found;
}

const bool Leaderboard::isOnLeaderboard(const uint32_t userID)
{
	return find(userID);
}

vector<GuestData>& Leaderboard::getGuests()
{
	return GuestDataList::getGuests();
}
