#include "GuestList.h"

void GuestList::setGuests(ParsecGuest* guests, int guestCount)
{
	stringstream comboStringStream;
	_guests.clear();

	for (size_t i = 0; i < guestCount; i++)
	{
		_guests.push_back
		(
			Guest
			(
				guests[i].name,
				guests[i].userID,
				guests[i].id
			)
		);

		if (i < GUESTLIST_MAX_GUESTS)
		{
			guestNames[i] = guests[i].name;
		}
	}
}

vector<Guest>& GuestList::getGuests()
{
    return _guests;
}

void GuestList::clear()
{
	_guests.clear();
}

const bool GuestList::find(uint32_t targetGuestID, Guest* result)
{
	vector<Guest>::iterator i;
	for (i = _guests.begin(); i != _guests.end(); ++i)
	{
		if ((*i).userID == targetGuestID)
		{
			*result = *i;
			return true;
		}
	}

	return false;
}

const bool GuestList::find(const char* targetName, Guest* result)
{
	return find(string(targetName), result);
}

const bool GuestList::find(string targetName, Guest* result)
{
	static const uint64_t MINIMUM_MATCH = 3;
	uint64_t closestDistance = STRINGER_MAX_DISTANCE;
	uint64_t distance = STRINGER_MAX_DISTANCE;
	bool found = false;

	if (targetName.length() < MINIMUM_MATCH)
	{
		return false;
	}

	vector<Guest>::iterator gi;
	for (gi = _guests.begin(); gi != _guests.end(); ++gi)
	{
		distance = Stringer::fuzzyDistance((*gi).name, targetName);
		if (distance <= closestDistance && distance <= STRINGER_DISTANCE_CHARS(MINIMUM_MATCH))
		{
			// If this is a draw, choose one based on following criteria...
			if (distance == closestDistance)
			{
				std::string candidateName = (*gi).name;
				std::string currentName = result->name;

				// If search tag is shorter than both
				if (targetName.length() < candidateName.length() && targetName.length() < currentName.length())
				{
					// Pick the shortest
					if (candidateName.length() < currentName.length())
					{
						*result = *gi;
					}
				}
				// If search tag is larger than any of them
				else
				{
					// Pick the largest
					if (candidateName.length() > currentName.length())
					{
						*result = *gi;
					}
				}
			}
			else
			{
				*result = *gi;
			}

			closestDistance = distance;
			found = true;
		}
	}

	return found;
}

MyMetrics GuestList::getMetrics(uint32_t id)
{
	auto it = _metrics.find(id);
	if (it != _metrics.end()) {
		return it->second;
	}
	MyMetrics defaultValues;
	return defaultValues;
}

void GuestList::deleteMetrics(uint32_t id)
{
	_metrics.erase(id);
}

void GuestList::updateMetrics(ParsecGuest* guests, int guestCount)
{
	for (size_t i = 0; i < guestCount; i++)
	{
		auto it = _metrics.find(guests[i].id);
		if (it != _metrics.end())
		{
			if (guests[i].metrics[0].slowRTs > it->second.metrics.slowRTs)
				it->second.congested = 2;
			else if (guests[i].metrics[0].fastRTs > it->second.metrics.fastRTs)
				it->second.congested = 1;
			else
				it->second.congested = 0;
			it->second.metrics.bitrate = guests[i].metrics[0].bitrate;
			it->second.metrics.decodeLatency = guests[i].metrics[0].decodeLatency;
			it->second.metrics.encodeLatency = guests[i].metrics[0].encodeLatency;
			it->second.metrics.fastRTs = guests[i].metrics[0].fastRTs;
			it->second.metrics.networkLatency = guests[i].metrics[0].networkLatency;
			it->second.metrics.packetsSent = guests[i].metrics[0].packetsSent;
			it->second.metrics.queuedFrames = guests[i].metrics[0].queuedFrames;
			it->second.metrics.slowRTs = guests[i].metrics[0].slowRTs;
			++it->second.averageNetworkLatencySize;
			it->second.averageNetworkLatency = it->second.averageNetworkLatency + (guests[i].metrics[0].networkLatency - it->second.averageNetworkLatency) / it->second.averageNetworkLatencySize;
		}
		else
		{
			_metrics.insert(pair<uint32_t, MyMetrics>(guests[i].id, { guests[i].metrics[0] } ));
		}
	}
}