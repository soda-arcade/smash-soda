#include "../Hosting.h"
extern Hosting g_hosting;
#include "Arcade.h"
#include "ScreenshotService.h"

// Singleton
Arcade Arcade::instance = Arcade();

/**
 * Constructor
 */
Arcade::Arcade() {

	// Custom artwork
	artwork = vector<Artwork>();

	// Load custom environment
	loadEnvironment();

}

/**
 * Loads a local environment file.
 */
bool Arcade::loadEnvironment() {
	const string configPath = PathHelper::GetConfigPath();
	if (configPath.empty()) {
		customEnvironment = false;
		return false;
	}

	const string filepath = configPath + "env.json";
	if (!MTY_FileExists(filepath.c_str())) {
		customEnvironment = false;
		return false;
	}

	size_t size = 0;
	void* data = MTY_ReadFile(filepath.c_str(), &size);
	if (data == nullptr || size == 0) {
		if (data != nullptr) {
			MTY_Free(data);
		}
		customEnvironment = false;
		return false;
	}

	try {
		const char* begin = static_cast<const char*>(data);
		json j = json::parse(begin, begin + size);
		MTY_Free(data);

		if (!j.contains("api_domain") || !j["api_domain"].is_string()) {
			customEnvironment = false;
			return false;
		}

		string loadedDomain = j["api_domain"].get<string>();
		_secure = true;
		_port = 0;

		const string httpsPrefix = "https://";
		const string httpPrefix = "http://";
		if (loadedDomain.rfind(httpsPrefix, 0) == 0) {
			loadedDomain = loadedDomain.substr(httpsPrefix.length());
			_secure = true;
		}
		else if (loadedDomain.rfind(httpPrefix, 0) == 0) {
			loadedDomain = loadedDomain.substr(httpPrefix.length());
			_secure = false;
		}

		if (j.contains("secure") && j["secure"].is_boolean()) {
			_secure = j["secure"].get<bool>();
		}

		size_t first = loadedDomain.find_first_not_of(" \t\r\n");
		if (first == string::npos) {
			customEnvironment = false;
			return false;
		}
		loadedDomain = loadedDomain.substr(first);

		const size_t slashPos = loadedDomain.find('/');
		if (slashPos != string::npos) {
			loadedDomain = loadedDomain.substr(0, slashPos);
		}

		while (!loadedDomain.empty() && (loadedDomain.back() == '/' || loadedDomain.back() == ' ' || loadedDomain.back() == '\t' || loadedDomain.back() == '\r' || loadedDomain.back() == '\n')) {
			loadedDomain.pop_back();
		}

		if (loadedDomain.empty()) {
			customEnvironment = false;
			return false;
		}

		const size_t colonPos = loadedDomain.rfind(':');
		if (colonPos != string::npos && colonPos + 1 < loadedDomain.size()) {
			const string portStr = loadedDomain.substr(colonPos + 1);
			if (!portStr.empty() && portStr.find_first_not_of("0123456789") == string::npos) {
				const int parsedPort = stoi(portStr);
				if (parsedPort > 0 && parsedPort <= 65535) {
					_port = static_cast<uint16_t>(parsedPort);
					loadedDomain = loadedDomain.substr(0, colonPos);
				}
			}
		}

		if (loadedDomain.empty()) {
			customEnvironment = false;
			return false;
		}

		domain = loadedDomain;
		customEnvironment = true;
		return true;
	}
	catch (const std::exception&) {
		MTY_Free(data);
		customEnvironment = false;
		return false;
	}

}

/**
 * Login to the arcade
 * @param email	Email address
 * @param password	Password
 * @return bool
 */
bool Arcade::login(string email, string password, string twoFactor = "") {

	// Create the JSON object
	json j;
	j["email"] = email;
	j["password"] = password;
	j["code"] = twoFactor;

	string data = j.dump();
	size_t bodySize = sizeof(char) * data.length();

	// Prepare the response
	void* response = nullptr;
	size_t responseSize = 0;

	// Send the request
	string path = "/api/auth/login";
	string method = "POST";

	const bool success = MTY_HttpRequest(
		domain.c_str(), _port, _secure, method.c_str(), path.c_str(),
		createHeaders(bodySize).c_str(),
		data.c_str(), bodySize, 20000,
		&response, &responseSize, &_status
	);

	const char* responseStr = (const char*)response;
	if (responseSize > 0 && _status == 200) {

		json result = json::parse(responseStr);

		Arcade::instance.credentials.token = result["token"];
		Arcade::instance.credentials.username = result["user"]["name"];

		// Save the file
		string configPath = PathHelper::GetConfigPath();
		if (configPath != "") {
			string filepath = configPath + "arcade.json";
			
			json j;
			j["token"] = Arcade::instance.credentials.token;
			j["username"] = Arcade::instance.credentials.username;

			string jsonStr = j.dump();
			std::ofstream file(filepath, std::ios::binary);
			if (file.is_open()) {
				std::string jsonStr = j.dump();
				xorEncryptDecrypt(jsonStr, _key);
				file.write(jsonStr.c_str(), jsonStr.size());
				file.close();
			}

		} else {

			error = responseStr;
			return false;

		}

		//MTY_Free(&response);
		return true;

	}

	//MTY_Free(&response);
	return false;
}

bool Arcade::loadCredentials() {

	string configPath = PathHelper::GetConfigPath();
	if (configPath != "") {
		string filepath = configPath + "arcade.json";
		if (MTY_FileExists(filepath.c_str())) {
			try {
				std::ifstream file(filepath, std::ios::binary); 
				if (file.is_open()) {
					std::ifstream file(filepath, std::ios::binary);
					std::stringstream buffer; 
					buffer << file.rdbuf(); 
					std::string encryptedStr = buffer.str(); 
					xorEncryptDecrypt(encryptedStr, _key);

					// Parse the json
					json j = json::parse(encryptedStr);

					Arcade::instance.credentials.token = j["token"];
					Arcade::instance.credentials.username = j["username"];
				}

				return true;

			}
			catch (exception e) {
				g_hosting.logMessage("Failed to load arcade credentials");
				return false;
			}
		}
		
	}

	return false;
}

bool Arcade::deleteCredentials() {

	string configPath = PathHelper::GetConfigPath();
	if (configPath != "") {
		string filepath = configPath + "arcade.json";
		if (MTY_FileExists(filepath.c_str())) {
			MTY_DeleteFile(filepath.c_str());
			return true;
		}
	}

	return false;
}

/**
 * Check if the token is valid
 * @param token	Token to check
 * @return bool
 */
bool Arcade::checkToken(string token) {
	if (!token.empty()) {
		Arcade::instance.credentials.token = token;
	}

	// Build the JSON string
	string data = "";
	size_t bodySize = sizeof(char) * data.length();
	string method = "GET";

	vector<string> validatePaths = {
		"/api/auth/me",
		"/api/me",
	};

	uint16_t lastStatus = 0;
	bool hadUnauthorized = false;

	for (const string& path : validatePaths) {
		void* response = nullptr;
		size_t responseSize = 0;

		const bool success = MTY_HttpRequest(
			domain.c_str(), _port, _secure, method.c_str(), path.c_str(),
			createHeaders(bodySize).c_str(),
			data.c_str(), bodySize, 20000,
			&response, &responseSize, &_status
		);

		lastStatus = _status;
		if (_status == 401 || _status == 403) {
			hadUnauthorized = true;
		}

		if (responseSize > 0 && _status == 200) {
			const char* responseStr = (const char*)response;
			try {
				// /me may include a new token, but client intentionally keeps the stored token.
				json result = json::parse(responseStr);
				if (result.contains("user") && result["user"].is_object() && result["user"].contains("name") && result["user"]["name"].is_string()) {
					Arcade::instance.credentials.username = result["user"]["name"].get<string>();
				}
			}
			catch (const std::exception&) {
				// Non-fatal: auth already succeeded by status code.
			}

			Config::cfg.arcade.showLogin = false;
			Config::cfg.Save();

			// Get the user's artwork
			getArtwork();
			g_hosting.logMessage("Logged in to Soda Arcade!");

			return true;
		}
	}

	if (hadUnauthorized) {
		Config::cfg.arcade.showLogin = true;
		Config::cfg.Save();
		deleteCredentials();
		return false;
	}

	// Route mismatch (404) should not spam logs when credentials are otherwise still valid for other endpoints.
	if (lastStatus != 404) {
		g_hosting.logMessage("Unable to validate Soda Arcade session. Error Code: " + to_string(lastStatus));
	}
	return false;

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
	string path = "/api/media?type=image&page=1";
	string method = "GET";

	const bool success = MTY_HttpRequest(
		domain.c_str(), _port, _secure, method.c_str(), path.c_str(),
		createHeaders(bodySize).c_str(),
		data.c_str(), bodySize, 20000,
		&response, &responseSize, &_status
	);

	const char* responseStr = (const char*)response;
	if (responseSize > 0 && _status == 200) {

		try {
			json result = json::parse(responseStr);
			if (!result.contains("data") || !result["data"].is_array()) {
				g_hosting.logMessage("Failed to load artwork: unexpected media response format.");
				return false;
			}

			artwork.clear();
			for (const auto& item : result["data"]) {
				Artwork art;
				art.id = item.value("id", 0);
				// New media model uses `name`; keep `title` fallback for compatibility.
				art.title = item.value("name", item.value("title", string("")));
				art.url = item.value("url", string(""));
				artwork.push_back(art);
			}

			g_hosting.logMessage(to_string(artwork.size()) + " artwork found.");
			return true;
		}
		catch (const exception&) {
			g_hosting.logMessage("Failed to parse artwork media response from Soda Arcade.");
			return false;
		}
	}

	if (_status == 401) {
		g_hosting.logMessage("Failed to retrieve artwork media: unauthorized.");
	}
	else {
		g_hosting.logMessage("A fatal error occured when trying to retrieve your custom artwork. If the issues persists, log out of Soda Arcade and back in.");
	}

	return false;
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

	// Create the JSON object (new room schema)
	json j;
	j["type"] = "parsec";

	string countryCode = "";
	for (const auto& country : countries.list) {
		if (country.first == Config::cfg.arcade.country || country.second == Config::cfg.arcade.country) {
			countryCode = country.first;
			break;
		}
	}

	if (countryCode.empty()) {
		const int countryIndex = Config::cfg.arcade.countryIndex;
		if (countryIndex >= 0 && countryIndex < static_cast<int>(countries.list.size())) {
			countryCode = countries.list[countryIndex].first;
		}
		else {
			countryCode = "US";
		}
	}

	j["name"] = Config::cfg.room.name;
	j["user_limit"] = Config::cfg.room.guestLimit;
	j["details"] = Config::cfg.room.details;
	j["stream_url"] = Config::cfg.room.streamUrl;

	// Only Parsec rooms are created from this client.
	j["meta"]["parsec"]["link"] = link;
	j["meta"]["parsec"]["peer_id"] = peer_id;
	j["meta"]["parsec"]["peer_secret"] = secret;
	j["meta"]["game"]["name"] = Config::cfg.room.game;
	j["meta"]["country"] = countryCode;
	j["meta"]["rep"] = Config::cfg.room.repThreshold;

	string previewType = Config::cfg.room.previewType;
	if (previewType != "auto" && previewType != "snapshot" && previewType != "custom") {
		previewType = "auto";
	}
	j["meta"]["info"]["previewType"] = previewType;

	// Send selected tags if any.
	if (!Config::cfg.room.tags.empty()) {
		j["meta"]["info"]["tags"] = Config::cfg.room.tags;
	}

	if (previewType == "custom") {
		// For custom preview, backend expects a concrete image URL.
		for (const auto& art : artwork) {
			if (art.id == artworkID && !art.url.empty()) {
				j["image"] = art.url;
				break;
			}
		}
	}

	string theme = Config::cfg.room.theme;
	std::transform(theme.begin(), theme.end(), theme.begin(), ::tolower);
	j["meta"]["theme"] = theme;

	// Build the JSON string
	string data = j.dump();
	size_t bodySize = sizeof(char) * data.length();

	// Prepare the response
	void* response = nullptr;
	size_t responseSize = 0;

	// Send the request
	string path = "/api/room";
	string method = "POST";

	const bool success = MTY_HttpRequest(
		domain.c_str(), _port, _secure, method.c_str(), path.c_str(),
		createHeaders(bodySize).c_str(),
		data.c_str(), bodySize, 20000,
		&response, &responseSize, &_status
	);

	const char* responseStr = (const char*)response;
	if (responseSize > 0 && (_status >= 200 && _status < 300)) {
		try {
			json result = json::parse(responseStr);

			// Store the room ID when available (create/update responses both include it).
			postID = result.value("id", -1);

			g_hosting.logMessage("Your room has been posted on https://soda-arcade.com");
			return true;
		}
		catch (const exception&) {
			g_hosting.logMessage("Failed to parse Soda Arcade room response.");
			return false;
		}
	}

	try {
		if (responseSize > 0) {
			json result = json::parse(responseStr);
			if (result.contains("message") && result["message"].is_string()) {
				g_hosting.logMessage("Failed to post on the arcade: " + result["message"].get<string>() + " (" + to_string(_status) + ")");
				return false;
			}
		}
	}
	catch (const exception&) {
	}

	g_hosting.logMessage("Failed to post on the arcade, for some reason. Please login again. Error Code: " + to_string(_status));
	return false;
}

/**
 * Delete a post on the arcade
 * @return bool
 */
bool Arcade::deletePost() {
	const int roomId = postID;
	if (roomId <= 0) {
		return false;
	}
	
	// Build the JSON string
	string data = "";
	size_t bodySize = sizeof(char) * data.length();

	// Prepare the response
	void* response = nullptr;
	size_t responseSize = 0;

	// Send the request
	string path = "/api/room/" + to_string(roomId);
	string method = "DELETE";

	const bool success = MTY_HttpRequest(
		domain.c_str(), _port, _secure, method.c_str(), path.c_str(),
		createHeaders(bodySize).c_str(),
		data.c_str(), bodySize, 20000,
		&response, &responseSize, &_status
	);

	const char* responseStr = (const char*)response;
	if (_status >= 200 && _status < 300) {
		postID = -1;
		return true;
	}
	else {
		return false;
	}
	return false;
}

/**
 * Delete a post on the arcade
 * @return bool
 */
bool Arcade::updateGuestCount(int guestCount) {

	// Create the JSON object
	json j;
	j["guest_count"] = guestCount;

	// Build the JSON string
	string data = j.dump();
	size_t bodySize = sizeof(char) * data.length();

	// Prepare the response
	void* response = nullptr;
	size_t responseSize = 0;

	// Send the request
	string path = "/api/room/guest/count";
	string method = "POST";

	const bool success = MTY_HttpRequest(
		domain.c_str(), _port, _secure, method.c_str(), path.c_str(),
		createHeaders(bodySize).c_str(),
		data.c_str(), bodySize, 20000,
		&response, &responseSize, &_status
	);

	const char* responseStr = (const char*)response;
	if (_status == 200) {
		//g_hosting.logMessage("Updated guest count to " + to_string(guestCount));
		return true;
	}
	else {
		return false;
	}
	return false;
}

/**
 * Capture and upload a room snapshot image to Soda Arcade.
 * @return bool
 */
bool Arcade::uploadSnapshot() {
	std::vector<uint8_t> jpegData;
	if (!ScreenshotService::getInstance().captureJpeg(jpegData, 300, 150, 85)) {
		return false;
	}

	// Build a multipart/form-data payload with the JPEG as `image`.
	const string boundary = "----SmashSodaSnapshotBoundary";
	const string preamble =
		"--" + boundary + "\r\n"
		"Content-Disposition: form-data; name=\"image\"; filename=\"snapshot.jpg\"\r\n"
		"Content-Type: image/jpeg\r\n\r\n";
	const string ending = "\r\n--" + boundary + "--\r\n";

	std::vector<uint8_t> body;
	body.reserve(preamble.size() + jpegData.size() + ending.size());
	body.insert(body.end(), preamble.begin(), preamble.end());
	body.insert(body.end(), jpegData.begin(), jpegData.end());
	body.insert(body.end(), ending.begin(), ending.end());

	void* response = nullptr;
	size_t responseSize = 0;

	string path = "/api/room/snapshot";
	string method = "POST";
	const string contentType = "multipart/form-data; boundary=" + boundary;

	const bool success = MTY_HttpRequest(
		domain.c_str(), _port, _secure, method.c_str(), path.c_str(),
		createHeaders(body.size(), contentType).c_str(),
		body.data(), body.size(), 20000,
		&response, &responseSize, &_status
	);

	if (response != nullptr) {
		MTY_Free(response);
	}

	return success && (_status >= 200 && _status < 300);
}

/**
 * Logout of the arcade
 * @return bool
 */
bool Arcade::logout() {
	Config::cfg.room.privateRoom = true;
	Config::cfg.Save();
	credentials.token = "";
	credentials.username = "";
	return deleteCredentials();
}

/**
 * Create headers for the request
 * @param size	Size of the body
 * @return string
 */
string Arcade::createHeaders(size_t size, const string& contentType) {
	string headers = "Content-Type: " + contentType + "\r\n";
	headers += "Content-Length: " + to_string(size) + "\r\n";
	headers += "User-Agent: SmashSoda\r\n";

	// Include token if set
	if (!Arcade::instance.credentials.token.empty()) {
		headers += "Authorization: Bearer " + Arcade::instance.credentials.token + "\r\n";
	}

	return headers;
};

void Arcade::xorEncryptDecrypt(std::string& data, const std::string& key) {
	for (size_t i = 0; i < data.size(); ++i) {
		data[i] ^= key[i % key.size()];
	}
}
