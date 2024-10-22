#include "../Hosting.h"
extern Hosting g_hosting;
#include "Arcade.h"

// Singleton
Arcade Arcade::instance = Arcade();

/**
 * Constructor
 */
Arcade::Arcade() {

	// Custom artwork
	artwork = vector<Artwork>();

	// Easy toggle for dev/prod
	_dev = false;
	_staging = false;

	// Which domain to use
	if (_dev) {
		_domain = _devDomain;
		_secure = false;
	} 
	else if (_staging) {
		_domain = _stagingDomain;
		_secure = true;
	}
	else {
		_domain = _prodDomain;
		_secure = true;
	}

}

/**
 * Login to the arcade
 * @param email	Email address
 * @param password	Password
 * @return bool
 */
bool Arcade::login(string email, string password) {

	// Create the JSON object
	json j;
	j["email"] = email;
	j["password"] = password;

	string data = j.dump();
	size_t bodySize = sizeof(char) * data.length();

	// Prepare the response
	void* response = nullptr;
	size_t responseSize = 0;

	// Send the request
	string path = "/api/login";
	string method = "POST";

	const bool success = MTY_HttpRequest(
		_domain.c_str(), 0, _secure, method.c_str(), path.c_str(),
		createHeaders(bodySize).c_str(),
		data.c_str(), bodySize, 20000,
		&response, &responseSize, &_status
	);

	const char* responseStr = (const char*)response;
	if (responseSize > 0 && _status == 200) {

		json result = json::parse(responseStr);

		Config::cfg.arcade.token = result["token"];
		Config::cfg.arcade.username = result["user"]["name"];

		//MTY_Free(&response);
		return true;

	}

	//MTY_Free(&response);
	return false;
}

/**
 * Check if the token is valid
 * @param token	Token to check
 * @return bool
 */
bool Arcade::checkToken(string token) {

	// Build the JSON string
	string data = "";
	size_t bodySize = sizeof(char) * data.length();

	// Prepare the response
	void* response = nullptr;
	size_t responseSize = 0;

	// Send the request
	string path = "/api/user/authenticate";
	string method = "POST";

	const bool success = MTY_HttpRequest(
		_domain.c_str(), 0, _secure, method.c_str(), path.c_str(),
		createHeaders(bodySize).c_str(),
		data.c_str(), bodySize, 20000,
		&response, &responseSize, &_status
	);

	const char* responseStr = (const char*)response;
	if (responseSize > 0 && _status == 200) {

		// Get the user's artwork
		getArtwork();

		return true;
	} else {
		Config::cfg.arcade.token = "";
		Config::cfg.arcade.showLogin = true;
		Config::cfg.Save();
		return false;
	}

}

/**
 * Get the user's artwork
 * @return bool
 */
bool Arcade::getArtwork() {

	// Build the JSON string
	string data = "";
	size_t bodySize = sizeof(char) * data.length();

	// Prepare the response
	void* response = nullptr;
	size_t responseSize = 0;

	// Send the request
	string path = "/api/artwork";
	string method = "GET";

	const bool success = MTY_HttpRequest(
		_domain.c_str(), 0, _secure, method.c_str(), path.c_str(),
		createHeaders(bodySize).c_str(),
		data.c_str(), bodySize, 20000,
		&response, &responseSize, &_status
	);

	const char* responseStr = (const char*)response;
	if (responseSize > 0 && _status == 200) {

		json result = json::parse(responseStr);
		artwork.clear();
		for (auto& item : result["data"]) {
			Artwork art;
			art.id = item["id"];
			art.title = item["title"];
			artwork.push_back(art);
		}
		g_hosting.logMessage(to_string(artwork.size()) + " artwork found.");

		return true;
	}
	else {
		Config::cfg.arcade.token = "";
		Config::cfg.arcade.showLogin = true;
		Config::cfg.Save();
		return false;
	}
}

/**
 * Create a post on the arcade
 * @return bool
 */
bool Arcade::createPost() {

	// Parsec session
	string peer_id = g_hosting.getSession().hostPeerId;
	string secret = Config::cfg.room.secret;
	string link = (string("https://parsec.gg/g/") + peer_id + "/" + secret + "/").c_str();

	// Create the JSON object
	json j;

	j["link"] = link;
	j["peer_id"] = peer_id;
	j["secret"] = secret;
	j["country"] = countries.list[Config::cfg.arcade.countryIndex].first;

	j["game"] = Config::cfg.room.game;
	j["guest_limit"] = Config::cfg.room.guestLimit;
	j["details"] = Config::cfg.room.details;
	j["theme"] = Config::cfg.room.theme;
	if (artworkID != -1) {
		j["artwork_id"] = artworkID;
	}

	// Build the JSON string
	string data = j.dump();
	size_t bodySize = sizeof(char) * data.length();

	// Prepare the response
	void* response = nullptr;
	size_t responseSize = 0;

	// Send the request
	string path = "/api/room/create/smash";
	string method = "POST";

	const bool success = MTY_HttpRequest(
		_domain.c_str(), 0, _secure, method.c_str(), path.c_str(),
		createHeaders(bodySize).c_str(),
		data.c_str(), bodySize, 20000,
		&response, &responseSize, &_status
	);

	const char* responseStr = (const char*)response;
	if (responseSize > 0 && (_status >= 200 && _status < 300)) {

		json result = json::parse(responseStr);
		g_hosting.logMessage("Your room has been posted on https://soda-arcade.com");

		//MTY_Free(&response);
		return true;

	}
	else {
		g_hosting.logMessage("Failed to post on the arcade, for some reason. Please login again. Error Code: " + to_string(_status));
		Config::cfg.arcade.token = "";
		Config::cfg.arcade.username = "";
		Config::cfg.arcade.showLogin = true;
		Config::cfg.Save();
	}
	return false;
}

/**
 * Delete a post on the arcade
 * @return bool
 */
bool Arcade::deletePost() {
	
	// Build the JSON string
	string data = "";
	size_t bodySize = sizeof(char) * data.length();

	// Prepare the response
	void* response = nullptr;
	size_t responseSize = 0;

	// Send the request
	string path = "/api/room/close";
	string method = "POST";

	const bool success = MTY_HttpRequest(
		_domain.c_str(), 0, _secure, method.c_str(), path.c_str(),
		createHeaders(bodySize).c_str(),
		data.c_str(), bodySize, 20000,
		&response, &responseSize, &_status
	);

	const char* responseStr = (const char*)response;
	if (responseSize > 0 && _status == 200) {
		//MTY_Free(&response);
		return true;
	}
	return false;
}

/**
 * Create headers for the request
 * @param size	Size of the body
 * @return string
 */
string Arcade::createHeaders(size_t size) {
	string headers = "Content-Type: application/json\r\n";
	headers += "Content-Length: " + to_string(size) + "\r\n";

	// Include token if set
	if (!Config::cfg.arcade.token.empty()) {
		headers += "Authorization: Bearer " + Config::cfg.arcade.token + "\r\n";
	}

	return headers;
};
