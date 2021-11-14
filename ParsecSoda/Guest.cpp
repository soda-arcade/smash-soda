#include "Guest.h"

Guest::Guest()
	: name(""), userID(0), id(0), metrics(), congested(false), status(Status::INVALID)
{}

Guest::Guest(string name, uint32_t userID, uint32_t id, ParsecMetrics metrics, bool congested, Status status)
	: name(name), userID(userID), id(id), metrics(metrics), congested(congested), status(status)
{}

Guest::Guest(ParsecGuest guest)
	: name(guest.name), userID(guest.userID), id(guest.id), metrics(guest.metrics[0]), congested(false), status(Status::OK)
{}

const bool Guest::isValid()
{
	return status == Status::OK;
}

Guest Guest::copy(const Guest& guest)
{
	name = guest.name;
	userID = guest.userID;
	id = guest.id;
	metrics = guest.metrics;
	congested = guest.congested;
	status = guest.status;
	return *this;
}