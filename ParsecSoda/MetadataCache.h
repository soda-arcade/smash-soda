#pragma once

#include <string>
#include <vector>
#include <mutex>
#include <algorithm>
#include <Windows.h>
#include <ShlObj.h>
#include "matoya.h"
#include "Guest.h"
#include "Models/GuestData.h"
#include "GameData.h"
#include "GuestTier.h"
#include "Thumbnail.h"
#include "Helpers/Stringer.h"
#include "Helpers/Stopwatch.h"

using namespace std;

class MetadataCache
{
public:

	/// <summary>
	/// Stores session info
	/// </summary>
	class SessionCache
	{
	public:
		enum class SessionType
		{
			PERSONAL = 0,
			THIRD = 1,
			ENTERPRISE = 2
		};

		string sessionID = "";
		string peerID = "";
		SessionType type = SessionType::THIRD;
		uint32_t start = 0;
		uint32_t expiry = 0;
		bool isValid = false;

	};

	/// <summary>
	/// Stores all the application and hosting preferences.
	/// </summary>
	class Preferences
	{
	public:
		unsigned int audioInputDevice = 0;
		unsigned int audioOutputDevice = 0;
		unsigned int micFrequency = 44100;
		unsigned int micVolume = 80;
		bool micEnabled = false;
		unsigned int speakersFrequency = 44100;
		unsigned int speakersVolume = 30;
		bool speakersEnabled = true;
		unsigned int monitor = 0;
		unsigned int adapter = 0;
		string roomName = "";
		string gameID = "";
		string secret = "";
		unsigned int guestCount = 1;
		bool publicRoom = false;
		bool isValid = false;
		int windowX = 0;
		int windowY = 0;
		unsigned int windowW = 1280;
		unsigned int windowH = 720;
		unsigned int fps = 60;
		unsigned int bandwidth = 20;
		unsigned int xboxPuppetCount = 4;
		unsigned int ds4PuppetCount = 0;
		bool basicVersion = false;
		bool disableMicrophone = false;
		bool disableGuideButton = false;
		bool disableKeyboard = false;
		string websocketURI = "";
		string websocketPassword = "";
		bool showMasterOfPuppets = false;
		bool showAudio = false;
		bool showVideo = false;
		bool showThumbs = false;
		bool showWebSocket = false;
		bool latencyLimitEnabled = false;
		unsigned int latencyLimitValue = 0;
		bool lockedGamepadLeftTrigger = false;
		bool lockedGamepadRightTrigger = false;
		bool lockedGamepadLX = false;
		bool lockedGamepadLY = false;
		bool lockedGamepadRX = false;
		bool lockedGamepadRY = false;
		unsigned int lockedGamepadButtons = 0;
		unsigned int theme = 0;
		string discord = "";

		bool hotseat = false;
		unsigned int hotseatTime = 15;
		bool hotseatAFK = false;
		unsigned int hotseatAFKTime = 5;
		bool hotseatPause = true;

		bool kioskMode = false;
		string kioskApplication = "";
		string kioskParameters = "";

		string chatbot = "ChatBot";
		string chatbotName = "";
		unsigned int muteTime = 5;
		bool autoMute = true;

		bool leaderboardEnabled = true;

		bool overlayShow = false;
		bool overlayHotseat = false;

		vector<Guest> activeGuests;

		class MutedGuest {
		public:
			uint32_t id;
			string name;
			Stopwatch stopwatch;
		};
		vector<MutedGuest> mutedGuests;

		int extraSpots = 0;
	
	};

	/// <summary>
	/// Stores all the kiosk mode information.
	/// </summary>
	class Kiosk {
	public:
		string filename = "";

		bool isRestarting = false;
	};

	/// <summary>
	/// This stores all information for the automatic gamepad
	/// commands.
	/// </summary>
	class AutoGamepad {
	public:
		int gamepadIndex = 0;
		vector<int> buttonList;

		bool isRunning = false;
		bool isPressed = false;
	};

	/// <summary>
	/// This stores all the hotseat and spectator information.
	/// </summary>
	class Hotseat {
	public:
		Guest guest;
		vector<uint32_t> spectators;

		Stopwatch hotseatClock;
		Stopwatch reminderClock;
		Stopwatch customClock;

		bool isRestarting = false;
		bool isWarning = false;
	};

	/// <summary>
	/// This stores all the team information for tournaments.
	/// </summary>
	class Tournament {
	public:

		class Team {
		public:
			vector<Guest> guests;
		};

		class Bracket {
		public:
			vector<Team> teams;
			vector<int> scores;

			int winningTeamIndex = 0;
			bool isOver = false;
		};

		class Round {
		public:
			vector<Bracket> brackets;

			int winningTeamIndex = 0;
			bool isOver = false;
			bool isStarted = false;
		};

		vector<string> guests;
		vector<Team> teams;
		vector<Round> rounds;

		bool isInit = false;
		bool isStarted = false;

		int type = 0;
		int teamSize = 0;

	};

	static SessionCache loadSessionCache();
	static bool saveSessionCache(SessionCache sessionCache);

	static Preferences loadPreferences();
	static bool savePreferences(Preferences preferences);
	static bool savePreferences();

	static vector<GuestData> loadBannedUsers();
	static bool saveBannedUsers(vector<GuestData> guests);

	static vector<GuestData> loadModdedUsers();
	static bool saveModdedUsers(vector<GuestData> guests);

	static vector<GuestData> loadVIPUsers();
	static bool saveVIPUsers(vector<GuestData> guests);

	static vector<GameData> loadGamesList();
	static bool saveGamesList(vector<GameData> games);

	static vector<GuestTier> loadGuestTiers();
	static bool saveGuestTiers(vector<GuestTier> guestTiers);

	static vector<Thumbnail> loadThumbnails();
	static bool saveThumbnails(vector<Thumbnail> thumbnails);

	static bool saveTheme(int theme);

	static bool isSpectating(Guest guest);

	static bool addActiveGuest(Guest guest);
	static bool removeActiveGuest(Guest guest);

	static Preferences preferences;
	static Kiosk kiosk;
	static AutoGamepad autoGamepad;
	static Hotseat hotseat;
	static Tournament tournament;

private:
	static string getUserDir();
	
	// This is not ideal, especially in an open source environment.
	// I'm using these values just as placeholders until I find an
	// actual solution. You should change them in your build.
	/** Must have 16 bytes. */
	static string _key;
	/** Must have 12 bytes. */
	static string _nonce;

	static mutex _mutex;
};