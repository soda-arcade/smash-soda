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
		name = "";
		path = "";
		parameters = "";
		thumbnailPath = "";
		gameID = 0;
	}

	/**
	 * Full constructor.
	 */
	GameData(string name, string path, string param, string thumb, uint32_t gameID) {
		set(name, path, param, thumb, gameID);
	}


	void set(string name, string path, string param, string thumb, uint32_t gameID) {
		this->name = name;
		this->path = path;
		this->parameters = param;
		this->thumbnailPath = thumb;
		this->gameID = gameID;
	}

	std::string name;
	std::string path;
	std::string parameters;
	std::string thumbnailPath;
	uint32_t gameID;

};

