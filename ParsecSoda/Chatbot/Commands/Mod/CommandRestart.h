#pragma once

#include "../Base/ACommand.h"
#include "../../../Modules/ProcessMan.h"

using namespace std;

class CommandRestart : public ACommand
{
public:
	string filename;

	/**
	 * @brief Construct a new CommandRestart object
	 * 
	 */
	CommandRestart() {}

	/**
	 * @brief Run the command
	 * 
	 * @return true
	 * @return false
	 */
	bool run() override {

		if (Config::cfg.kioskMode.enabled) {

			SetReply("The application is restarting...wait!");
			ProcessMan::instance.restart();

		}
		else {
			SetReply("The host does not have kiosk mode enabled.");
		}
		
		return true;
	}

	/**
	 * @brief Get the prefixes object
	 * 
	 * @return std::vector<const char*>
	 */
	static vector<const char*> prefixes() {
		return vector<const char*> { "!restart", "!kill" };
	}
};