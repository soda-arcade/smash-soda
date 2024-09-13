#pragma once

#include "../Lists/VerifiedList.h"
#include "../Lists/BanList.h"
#include "../Lists/ModList.h"
#include "../Lists/TierList.h"
#include "../Lists/GameDataList.h"
#include "../Lists/VIPList.h"
#include "../Lists/SFXList.h"

struct IPRange {
    uint32_t start;
    uint32_t end;

    IPRange(const std::string& cidr) {
        size_t pos = cidr.find('/');
        std::string ip = cidr.substr(0, pos);
        int prefix = std::stoi(cidr.substr(pos + 1));

        uint32_t ip_num = ipToUint(ip);
        uint32_t mask = (prefix == 0) ? 0 : (~0U << (32 - prefix));

        start = ip_num & mask;
        end = start | ~mask;
    }

    static uint32_t ipToUint(const std::string& ip) {
        std::istringstream iss(ip);
        std::string token;
        uint32_t result = 0;

        for (int i = 0; i < 4; ++i) {
            std::getline(iss, token, '.');
            result = (result << 8) | std::stoi(token);
        }

        return result;
    }
};

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

    string version = "5.1.0"; // Current version of Smash Soda

    VerifiedList verifiedList; // List of verified users
    BanList banList; // List of banned users
    ModList modList; // List of moderators
    TierList tierList; // List of tiers
    VIPList vipList; // List of VIPs
    GameDataList gameList; // Host's list of games
    SFXList sfxList; // List of SFX

	std::string pendingIpAddress; // This holds the last recorded IP address of a user
    std::string lastIpAddress; // This holds the last recorded IP address of a user
    std::unordered_map<uint32_t, std::string> userIpMap; // Map of user IDs to IP addresses
    vector<string> bannedIPs = vector<string>(); // List of banned IP addresses
    vector<IPRange> cidrRanges = vector<IPRange>(); // List of CIDR ranges for VPNs

    vector<uint32_t> sodaCops = vector<uint32_t>(); // List of Soda cops! These users have mod powers in any room
    vector<uint32_t> globalBans = vector<uint32_t>(); // List of users who are banned from all rooms

    bool checkForUpdates();
    
    void banIPAddress(std::string ip);
	void unbanIPAddress(std::string ip);
    void unbanLastIPAddress();
	bool isBannedIPAddress(std::string ip);
	std::string getUserIpAddress(uint32_t userId);

    void getVPNList();
    bool isVPN(const string& ipStr);

    void addSodaCop(uint32_t userId);
    bool isSodaCop(uint32_t userId);
    bool isMickey(uint32_t userId);

    void addGlobalBan(uint32_t userId);
    bool isGlobalBanned(uint32_t userId);

    Cache();
private:
    void LoadBannedIpAddresses();
};