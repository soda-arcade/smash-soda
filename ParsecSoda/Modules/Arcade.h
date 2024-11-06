#pragma once
#include <nlohmann/json.hpp>
#include "matoya.h"
#include "../Helpers/Countries.h"
#include "../Helpers/PathHelper.h"

using namespace std;

/// <summary>
/// Handles Soda Arcade stuff
/// </summary>
class Arcade {
private:
	bool _dev = false;		/// Whether or not to use the dev server
	int _postID = -1;		/// The ID of the post associated with the room
	bool _secure = false;	/// Whether or not to use HTTPS
	uint16_t _status = 0;	/// The HTTP status code of the last request
	string _key = "AllWorkAndNoPlayMakesMickeyADullBoy**";

	string _domain; /// The domain of the server
	string _prodDomain = "soda-arcade.com"; /// The domain of the production server

	string createHeaders(size_t size);
	void xorEncryptDecrypt(std::string& data, const std::string& key);
public:
	class Artwork {
	public:
		int id = 0;
		string title = "";
	};

	class Credentials {
	public:
		string token = "";
		string username = "";
	};

	Credentials credentials = Credentials();
	static Arcade instance;
	vector<Artwork> artwork = vector<Artwork>();
	Countries countries = Countries();
	int artworkID = -1;	/// The ID of the artwork associated with the room

	Arcade();
	
	bool loadCredentials();
	bool deleteCredentials();
	bool logout();
	bool login(string email, string password, string twoFactor);
	bool checkToken(string token);

	bool createPost();
	bool deletePost();

	bool updateGuestCount(int count);

	bool getArtwork();
};