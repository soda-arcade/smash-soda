#include "../Hosting.h"
extern Hosting g_hosting;
#include "Arcade.h"

// Singleton
Arcade Arcade::instance = Arcade();

/// <summary>
/// Constructor
/// </summary>
Arcade::Arcade() {

	// Easy toggle for dev/prod
	_dev = false;

	// Which domain to use
	if (_dev) {
		_domain = _devDomain;
		_secure = false;
	} else {
		_domain = _prodDomain;
		_secure = true;
	}

}

/// <summary>
/// Login to the arcade and store the user's token.
/// </summary>
/// <param name="email"></param>
/// <param name="password"></param>
/// <returns></returns>
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

/// <summary>
/// Create a new post on the arcade
/// </summary>
bool Arcade::createPost() {

	// Parsec session
	string peer_id = g_hosting.getSession().hostPeerId;
	string secret = Config::cfg.room.secret;
	string link = (string("https://parsec.gg/g/") + peer_id + "/" + secret + "/").c_str();

	// Create the JSON object
	json j;

	// Are we updating or creating a new post?
	if (_postID != -1) {
		j["id"] = _postID;
	}

	j["type"] = "soda";
	j["link"] = link;
	j["peer_id"] = peer_id;
	j["secret"] = secret;
	j["slug"] = Config::cfg.room.secret;

	j["game"] = Config::cfg.room.game;
	j["private"] = Config::cfg.room.privateRoom;
	j["guest_limit"] = Config::cfg.room.guestLimit;
	j["details"] = Config::cfg.room.details;
	//j["latency_limit"] = (Config::cfg.room.latencyLimit ? Config::cfg.room.latencyLimitThreshold : 0);
	j["theme"] = Config::cfg.room.theme;
	//j["artwork_id"] = Config::cfg.room.artworkID;

	// Build the JSON string
	string data = j.dump();
	size_t bodySize = sizeof(char) * data.length();

	// Prepare the response
	void* response = nullptr;
	size_t responseSize = 0;

	// Send the request
	string path = "/api/post/smash";
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
		_postID = result["id"];
		g_hosting.logMessage("Your room has been posted on https://soda-arcade.com");

		//MTY_Free(&response);
		return true;

	}
	else {
		g_hosting.logMessage("Failed to post on the arcade, for some reason. Please login again.");
		Config::cfg.arcade.token = "";
		Config::cfg.arcade.username = "";
		Config::cfg.arcade.showLogin = true;
	}

	//MTY_Free(&response);
	return false;
}

/// <summary>
/// Delete a post on the arcade
/// </summary>
/// <returns></returns>
bool Arcade::deletePost() {
	
	// If we don't have a post ID, we can't delete
	if (_postID == -1) {
		return false;
	}
	
	// Create the JSON object
	json j;

	j["post_id"] = _postID;

	// Build the JSON string
	string data = j.dump();
	size_t bodySize = sizeof(char) * data.length();

	// Prepare the response
	void* response = nullptr;
	size_t responseSize = 0;

	// Send the request
	string path = "/api/post";
	string method = "DELETE";

	const bool success = MTY_HttpRequest(
		_domain.c_str(), 0, _secure, method.c_str(), path.c_str(),
		createHeaders(bodySize).c_str(),
		data.c_str(), bodySize, 20000,
		&response, &responseSize, &_status
	);

	const char* responseStr = (const char*)response;
	if (responseSize > 0 && _status == 200) {

		_postID = -1;

		//MTY_Free(&response);
		return true;

	}

	//MTY_Free(&response);
	return false;
}

/// <summary>
/// Update a post on the arcade
/// </summary>
/// <returns></returns>
bool Arcade::updatePost() {
	return true;
}

/// <summary>
/// Create the headers for the arcade request
/// </summary>
/// <param name="size"></param>
/// <returns></returns>
string Arcade::createHeaders(size_t size) {
	string headers = "Content-Type: application/json\r\n";
	headers += "Content-Length: " + to_string(size) + "\r\n";

	// Include token if set
	if (!Config::cfg.arcade.token.empty()) {
		headers += "Authorization: Bearer " + Config::cfg.arcade.token + "\r\n";
	}

	return headers;
};