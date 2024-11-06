#pragma once

#include "../../ACommand.h"
#include "../../../Modules/Hotseat.h"

class CommandCooldown : public ACommand
{
public:

	std::string usage = "Usage: !cooldown <username> <minutes>\nExample: !cooldown Altearis 6\0";

	/**
	 * @brief Construct a new CommandExtendCD object
	 *
	 * @param msg
	 * @param sender
	 * @param guests
	 * @param host
	 */
	CommandCooldown(const char* msg, Guest& sender, GuestList& guests, Guest& host)
		: ACommand(msg, sender), _host(host), guests(guests)
	{}

	/**
	 * @brief Run the command
	 *
	 * @return true
	 * @return false
	 */
	bool run() override {

		// Was a guest specified?
		if (getArgs().size() < 2) {
			setReply(usage);
			return false;
		}

		// Find the guest
		if (!findGuest()) {
			setReply("Can't find the guest!\0");
			return false;
		}

		int amount = 0;
		try {
			amount = std::stoi(getArgs()[1]);
		}
		catch (std::invalid_argument) {
			setReply(usage);
			return false;
		}
		Hotseat::instance.increaseUserCooldown(target.userID, amount);
		setReply("Cooldown extended!\0");

		return false;
	}

	/**
	 * @brief Get the prefixes object
	 *
	 * @return std::vector<const char*>
	 */
	static vector<const char*> prefixes() {
		return vector<const char*> { "!cooldown", "!screw" };
	}

protected:
	static vector<const char*> internalPrefixes() {
		return vector<const char*> { "!cooldown " };
	}

	/**
	* Get the guest referenced in the command. Returns nullptr
	* if no guest is found
	*
	* @param guestList The guest list
	*/
	bool findGuest() {

		// Get the guest
		string guest = getArgs().size() > 0 ? getArgs()[0] : "";
		if (guest == "") {
			return false;
		}

		try {
			uint32_t id = stoul(guest);
			vector<Guest>::iterator i;
			for (i = guests.getGuests().begin(); i != guests.getGuests().end(); ++i) {
				if ((*i).userID == id) {
					target = *i;
					return true;
				}
			}
		}
		catch (const std::exception&) {
			bool found = guests.find(guest, &target);
			if (found) {
				return true;
			}
		}

		return false;
	}

	Guest target;
	GuestList guests;

	Guest& _host;
};