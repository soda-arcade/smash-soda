#pragma once

#include "../Lists/VerifiedList.h"
#include "../Lists/BanList.h"
#include "../Lists/ModList.h"
#include "../Lists/TierList.h"
#include "../Lists/GameDataList.h"
#include "../Lists/VIPList.h"
#include "../Lists/SFXList.h"

/**
 * @brief Cache class
 * This class is used to store data that is frequently accessed.
 */
class Cache {
public:
    class Update {
	public:
		string version = ""; 				// The latest version available
		string notes = ""; 					// The markdown notes for the latest version
		bool overlay = false; 				// Whether the overlay files need updating
		bool critical = true; 				// Whether the update is critical
	};

    class Artwork {
	public:
		int id = 0;
		string title = "";
	};

	vector<Artwork> artwork = vector<Artwork>(); // List of artwork IDs and titles

    static Cache cache;
    Update update; // Update information

    string version = "5.0.0"; // Current version of Smash Soda

    VerifiedList verifiedList; // List of verified users
    BanList banList; // List of banned users
    ModList modList; // List of moderators
    TierList tierList; // List of tiers
    VIPList vipList; // List of VIPs
    GameDataList gameList; // Host's list of games
    SFXList sfxList; // List of SFX

	std::string pendingIpAddress; // This holds the last recorded IP address of a user
    std::unordered_map<uint32_t, std::string> userIpMap; // Map of user IDs to IP addresses
    vector<string> bannedIPs = vector<string>(); // List of banned IP addresses

    vector<uint32_t> sodaCops = vector<uint32_t>(); // List of Soda cops! These users have mod powers in any room
    vector<uint32_t> globalBans = vector<uint32_t>(); // List of users who are banned from all rooms

    bool checkForUpdates();
    
    void banIPAddress(std::string ip);
	void unbanIPAddress(std::string ip);
	bool isBannedIPAddress(std::string ip);
	std::string getUserIpAddress(uint32_t userId);

    void addSodaCop(uint32_t userId);
    bool isSodaCop(uint32_t userId);
    bool isMickey(uint32_t userId);

    void addGlobalBan(uint32_t userId);
    bool isGlobalBanned(uint32_t userId);

    Cache();
private:
    void LoadBannedIpAddresses();
};