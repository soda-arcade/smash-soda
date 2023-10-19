#pragma once

#include <iostream>

using namespace std;

/**
 *  A reduced data structure to hold basic guest data.
 */
class GameData {
public:

	/**
	 *  Empty constructor.
	 */
	GameData() {
		itemID = 0;
		name = "";
		path = "";
		parameters = "";
		thumbnailPath = "";
		gameID = "";
		kiosk = false;
		hotseat = false;
		seats = 0;
	}

	/**
	 * Full constructor.
	 */
	GameData(uint32_t itemID, string name, string path, string param, string thumb, string gameID, bool kiosk, bool hotseat, uint32_t seats) {
		set(itemID, name, path, param, thumb, gameID, kiosk, hotseat, seats);
	}


	void set(uint32_t itemID, string name, string path, string param, string thumb, string gameID, bool kiosk, bool hotseat, uint32_t seats) {
		this->itemID = itemID;
		this->name = name;
		this->path = path;
		this->parameters = param;
		this->thumbnailPath = thumb;
		this->gameID = gameID;
		this->kiosk = kiosk;
		this->hotseat = hotseat;
		this->seats = seats;
	}

	uint32_t itemID;
	std::string name;
	std::string path;
	std::string parameters;
	std::string thumbnailPath;
	std::string gameID;
	bool kiosk;
	bool hotseat;
	uint32_t seats;

};

