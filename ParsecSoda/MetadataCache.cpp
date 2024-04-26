#include "MetadataCache.h"

// This is not ideal, especially in an open source environment.
// I'm using these values just as placeholders until I find an
// actual solution. You should change them in your build.
string MetadataCache::_key = "ParsecSodaKey***";
string MetadataCache::_nonce = "ParsecSoda**";

MetadataCache::Preferences MetadataCache::preferences = MetadataCache::Preferences();
MetadataCache::Hotseat MetadataCache::hotseat = MetadataCache::Hotseat();
MetadataCache::AutoGamepad MetadataCache::autoGamepad = MetadataCache::AutoGamepad();
MetadataCache::Kiosk MetadataCache::kiosk = MetadataCache::Kiosk();
MetadataCache::Tournament MetadataCache::tournament = MetadataCache::Tournament();

mutex MetadataCache::_mutex;

MetadataCache::SessionCache MetadataCache::loadSessionCache()
{
    SessionCache result = SessionCache();

    string dirPath = getUserDir();

    if (!dirPath.empty())
    {
        string filepath = dirPath + "session.json";

        if (MTY_FileExists(filepath.c_str()))
        {
            size_t size;
            const char* encryptedContent;
            encryptedContent = (char*)MTY_ReadFile(filepath.c_str(), &size);

            char originalText[256];
            MTY_AESGCM* ctx = MTY_AESGCMCreate(_key.c_str());
            char tag[16] = "ParsecSodaTag**";

            MTY_AESGCMDecrypt(ctx, _nonce.c_str(), encryptedContent, size, tag, (void*)originalText);
            MTY_JSON* json = MTY_JSONParse(originalText);
            if (json != nullptr)
            {
                char sessionID[128];
                char peerID[64];
                uint32_t type = (uint32_t)SessionCache::SessionType::THIRD;
                uint32_t expiry = 0, start = 0;

                bool success =
                    MTY_JSONObjGetString(json, "sessionID", sessionID, 128)
                    && MTY_JSONObjGetString(json, "peerID", peerID, 64)
                    && MTY_JSONObjGetUInt(json, "type", &type)
                    && MTY_JSONObjGetUInt(json, "start", &start)
                    && MTY_JSONObjGetUInt(json, "expiry", &expiry)
                    ;

                if (success)
                {
                    result.isValid = true;
                    result.sessionID = sessionID;
                    result.peerID = peerID;
                    result.type = (SessionCache::SessionType)type;
                    result.start = start;
                    result.expiry = expiry;
                }

                MTY_JSONDestroy(&json);
            }

            MTY_AESGCMDestroy(&ctx);
        }
    }

    return result;
}

bool MetadataCache::saveSessionCache(SessionCache sessionCache)
{
    bool result = false;
    string dirPath = getUserDir();

    if (!dirPath.empty())
    {
        string filepath = dirPath + "session.json";

        if (sessionCache.isValid)
        {
            MTY_JSON* json = MTY_JSONObjCreate();
            MTY_JSONObjSetString(json, "sessionID", sessionCache.sessionID.c_str());
            MTY_JSONObjSetString(json, "peerID", sessionCache.peerID.c_str());
            MTY_JSONObjSetUInt(json, "type", (uint32_t)sessionCache.type);
            MTY_JSONObjSetUInt(json, "start", sessionCache.start);
            MTY_JSONObjSetUInt(json, "expiry", sessionCache.expiry);

            string jsonStr = MTY_JSONSerialize(json);
            char encryptedJson[256];
            char tag[16] = "ParsecSodaTag**";
            MTY_AESGCM* ctx = MTY_AESGCMCreate(_key.c_str());
            if (MTY_AESGCMEncrypt(ctx, _nonce.c_str(), jsonStr.c_str(), jsonStr.size(), tag, encryptedJson))
            {
                if (MTY_WriteFile(filepath.c_str(), encryptedJson, jsonStr.size()))
                {
                    result = true;
                }
            }

            MTY_AESGCMDestroy(&ctx);
            MTY_JSONDestroy(&json);
        }
    }

    return result;
}

MetadataCache::Preferences MetadataCache::loadPreferences()
{
    preferences = Preferences();

    string dirPath = getUserDir();
    if (!dirPath.empty())
    {
        string filepath = dirPath + "preferences.json";

        if (MTY_FileExists(filepath.c_str()))
        {
            MTY_JSON* json = MTY_JSONReadFile(filepath.c_str());
            char roomName[256] = "", gameID[72] = "", secret[32] = "";
            char websocketURI[50] = "";
            char websocketPassword[32] = "";
            char discord[128] = "";
            char kioskApplication[256] = "";
            char kioskParameters[256] = "";
            char chatbot[128] = "ChatBot";
            char welcomeMessage[256] = "";
			char region[128] = "";

            if (!MTY_JSONObjGetUInt(json, "audioInputDevice", &preferences.audioInputDevice)) {
                preferences.audioInputDevice = 0;
            }

            if (!MTY_JSONObjGetUInt(json, "audioOutputDevice", &preferences.audioOutputDevice)) {
                preferences.audioOutputDevice = 0;
            }

            if (!MTY_JSONObjGetUInt(json, "micFrequency", &preferences.micFrequency)) {
                preferences.micFrequency = 44100;
            }

            if (!MTY_JSONObjGetUInt(json, "micVolume", &preferences.micVolume)) {
                preferences.micVolume = 80;
            }

            if (!MTY_JSONObjGetBool(json, "micEnabled", &preferences.micEnabled)) {
                preferences.micEnabled = true;
            }

            if (!MTY_JSONObjGetUInt(json, "speakersFrequency", &preferences.speakersFrequency)) {
                preferences.speakersFrequency = 44100;
            }

            if (!MTY_JSONObjGetUInt(json, "speakersVolume", &preferences.speakersVolume)) {
                preferences.speakersVolume = 30;
            }

            if (!MTY_JSONObjGetBool(json, "speakersEnabled", &preferences.speakersEnabled)) {
                preferences.speakersEnabled = true;
            }

            if (!MTY_JSONObjGetUInt(json, "monitor", &preferences.monitor)) {
                preferences.monitor = 0;
            }

            if (!MTY_JSONObjGetUInt(json, "adapter", &preferences.adapter)) {
                preferences.adapter = 0;
            }

            if (MTY_JSONObjGetString(json, "roomName", roomName, 256)) preferences.roomName = roomName;
            else preferences.roomName = "Let's have fun!";

            if (MTY_JSONObjGetString(json, "gameID", gameID, 72)) preferences.gameID = gameID;
            else preferences.gameID = "";

            if (MTY_JSONObjGetString(json, "secret", secret, 32)) preferences.secret = secret;
            else preferences.secret = "";

            if (!MTY_JSONObjGetUInt(json, "guestCount", &preferences.guestCount)) {
                preferences.guestCount = 1;
            }

            if (!MTY_JSONObjGetBool(json, "publicRoom", &preferences.publicRoom)) {
                preferences.publicRoom = true;
            }

            static int LIMIT_POS = 4096 * 2;
            if (!MTY_JSONObjGetInt(json, "windowX", &preferences.windowX) || preferences.windowX < -LIMIT_POS || preferences.windowX > LIMIT_POS) {
                preferences.windowX = 0;
            }

            if (!MTY_JSONObjGetInt(json, "windowY", &preferences.windowY) || preferences.windowY < -LIMIT_POS || preferences.windowY > LIMIT_POS) {
                preferences.windowY = 0;
            }

            if (!MTY_JSONObjGetUInt(json, "windowW", &preferences.windowW) || preferences.windowW < 400) {
                preferences.windowW = 1280;
            }

            if (!MTY_JSONObjGetUInt(json, "windowH", &preferences.windowH) || preferences.windowH < 400) {
                preferences.windowH = 720;
            }

            if (!MTY_JSONObjGetUInt(json, "fps", &preferences.fps)) {
                preferences.fps = 60;
            }

            if (!MTY_JSONObjGetUInt(json, "bandwidth", &preferences.bandwidth)) {
                preferences.bandwidth = 20;
            }

            if (!MTY_JSONObjGetUInt(json, "xboxPuppetCount", &preferences.xboxPuppetCount)) {
                preferences.xboxPuppetCount = 4;
            }

            if (!MTY_JSONObjGetUInt(json, "ds4PuppetCount", &preferences.ds4PuppetCount)) {
                preferences.ds4PuppetCount = 0;
            }

            if (!MTY_JSONObjGetBool(json, "basicVersion", &preferences.basicVersion)) preferences.basicVersion = false;
            if (!MTY_JSONObjGetBool(json, "disableMicrophone", &preferences.disableMicrophone)) preferences.disableMicrophone = false;
            if (!MTY_JSONObjGetBool(json, "disableGuideButton", &preferences.disableGuideButton)) preferences.disableGuideButton = true;
            if (!MTY_JSONObjGetBool(json, "disableKeyboard", &preferences.disableKeyboard)) preferences.disableKeyboard = false;

            if (MTY_JSONObjGetString(json, "websocketURI", websocketURI, 50)) preferences.websocketURI = websocketURI;
            else preferences.websocketURI = "ws://127.0.0.1:9002";

            if (MTY_JSONObjGetString(json, "websocketPassword", websocketPassword, 32)) preferences.websocketPassword = websocketPassword;
            else preferences.websocketPassword = "";

			if (!MTY_JSONObjGetBool(json, "showHostSettings", &preferences.showHostSettings)) preferences.showHostSettings = true;
			if (!MTY_JSONObjGetBool(json, "showChat", &preferences.showChat)) preferences.showChat = true;
			if (!MTY_JSONObjGetBool(json, "showLog", &preferences.showLog)) preferences.showLog = false;
			if (!MTY_JSONObjGetBool(json, "showGuests", &preferences.showGuests)) preferences.showGuests = false;
			if (!MTY_JSONObjGetBool(json, "showGamepads", &preferences.showGamepads)) preferences.showGamepads = true;
            if (!MTY_JSONObjGetBool(json, "showMasterOfPuppets", &preferences.showMasterOfPuppets)) preferences.showMasterOfPuppets = false;
            if (!MTY_JSONObjGetBool(json, "showAudio", &preferences.showAudio)) preferences.showAudio = false;
            if (!MTY_JSONObjGetBool(json, "showVideo", &preferences.showVideo)) preferences.showVideo = false;
			if (!MTY_JSONObjGetBool(json, "showSettings", &preferences.showSettings)) preferences.showSettings = false;
			if (!MTY_JSONObjGetBool(json, "showButtonLock", &preferences.showButtonLock)) preferences.showButtonLock = false;
			if (!MTY_JSONObjGetBool(json, "showLibrary", &preferences.showLibrary)) preferences.showLibrary = false;
			if (!MTY_JSONObjGetBool(json, "showHotseat", &preferences.showHotseat)) preferences.showHotseat = false;
            if (!MTY_JSONObjGetBool(json, "lockedGamepadLeftTrigger", &preferences.lockedGamepadLeftTrigger)) preferences.lockedGamepadLeftTrigger = false;
            if (!MTY_JSONObjGetBool(json, "lockedGamepadRightTrigger", &preferences.lockedGamepadRightTrigger)) preferences.lockedGamepadRightTrigger = false;
            if (!MTY_JSONObjGetBool(json, "lockedGamepadLX", &preferences.lockedGamepadLX)) preferences.lockedGamepadLX = false;
            if (!MTY_JSONObjGetBool(json, "lockedGamepadLY", &preferences.lockedGamepadLY)) preferences.lockedGamepadLY = false;
            if (!MTY_JSONObjGetBool(json, "lockedGamepadRX", &preferences.lockedGamepadRX)) preferences.lockedGamepadRX = false;
            if (!MTY_JSONObjGetBool(json, "lockedGamepadRY", &preferences.lockedGamepadRY)) preferences.lockedGamepadRY = false;
            if (!MTY_JSONObjGetUInt(json, "lockedGamepadButtons", &preferences.lockedGamepadButtons)) preferences.lockedGamepadButtons = 0;
            if (!MTY_JSONObjGetUInt(json, "theme", &preferences.theme)) preferences.theme = 0;

            if (MTY_JSONObjGetString(json, "discord", discord, 128)) preferences.discord = discord;
            else preferences.discord = "";

            // HOTSEAT
            if (!MTY_JSONObjGetBool(json, "hotseatSlotMatch", &preferences.hotseatSlotMatch)) preferences.hotseatSlotMatch = true;
            if (!MTY_JSONObjGetUInt(json, "hotseatSeats", &preferences.hotseatSeats)) preferences.hotseatSeats = 1;
            if (!MTY_JSONObjGetUInt(json, "hotseatTime", &preferences.hotseatTime)) preferences.hotseatTime = 15;
            if (!MTY_JSONObjGetBool(json, "hotseatAFK", &preferences.hotseatAFK)) preferences.hotseatAFK = false;
            if (!MTY_JSONObjGetUInt(json, "hotseatAFKTime", &preferences.hotseatAFKTime)) preferences.hotseatAFKTime = 5;
            if (!MTY_JSONObjGetBool(json, "hotseatPause", &preferences.hotseatPause)) preferences.hotseatPause = true;

            // KIOSK MODE
            if (!MTY_JSONObjGetBool(json, "kioskMode", &preferences.kioskMode)) preferences.kioskMode = false;
            if (MTY_JSONObjGetString(json, "kioskApplication", kioskApplication, 256)) preferences.kioskApplication = kioskApplication;
            else preferences.kioskApplication = "";
            if (MTY_JSONObjGetString(json, "kioskParameters", kioskParameters, 256)) preferences.kioskParameters = kioskParameters;
            else preferences.kioskParameters = "";

            // Chat
            if (MTY_JSONObjGetString(json, "chatbot", chatbot, 128)) preferences.chatbot = chatbot;
            else preferences.chatbot = "ChatBot";

            if (!MTY_JSONObjGetUInt(json, "muteTime", &preferences.muteTime)) preferences.muteTime = 5;
            if (!MTY_JSONObjGetBool(json, "autoMute", &preferences.autoMute)) preferences.autoMute = false;
            if (!MTY_JSONObjGetUInt(json, "autoMuteTime", &preferences.autoMuteTime)) preferences.autoMuteTime = 500;
            if (!MTY_JSONObjGetBool(json, "saveLog", &preferences.saveLog)) preferences.saveLog = false;

            if (MTY_JSONObjGetString(json, "welcomeMessage", welcomeMessage, 256)) preferences.welcomeMessage = welcomeMessage;
            else preferences.welcomeMessage = "";

            preferences.chatbotName = "[" + preferences.chatbot + "]";

            // Tournament
            if (!MTY_JSONObjGetBool(json, "leaderboardEnabled", &preferences.leaderboardEnabled)) preferences.leaderboardEnabled = true;

            // Overlay
            if (!MTY_JSONObjGetBool(json, "overlayShow", &preferences.overlayShow)) preferences.overlayShow = false;
            if (!MTY_JSONObjGetBool(json, "overlayHotseat", &preferences.overlayHotseat)) preferences.overlayHotseat = false;

            // VIP
            if (!MTY_JSONObjGetBool(json, "vipBB", &preferences.vipBB)) preferences.vipBB = true;
            if (!MTY_JSONObjGetBool(json, "vipTournament", &preferences.vipTournament)) preferences.vipTournament = true;

            // Hosting
            if (!MTY_JSONObjGetUInt(json, "selectedGame", &preferences.selectedGame)) preferences.selectedGame = 0;
            if (MTY_JSONObjGetString(json, "prependRegion", region, 128)) preferences.prependRegion = region;
            else preferences.prependRegion = "";

            if (!MTY_JSONObjGetBool(json, "prependPingLimit", &preferences.prependPingLimit)) preferences.prependPingLimit = true;

            if (!MTY_JSONObjGetBool(json, "firstStartup", &preferences.firstStartup)) {
                preferences.firstStartup = true;
            }

            preferences.isValid = true;

            MTY_JSONDestroy(&json);
        }
    }

    return preferences;
}

bool MetadataCache::savePreferences(MetadataCache::Preferences preferences)
{
    if (!preferences.isValid)
    {
        return false;
    }

    string dirPath = getUserDir();
    if (!dirPath.empty())
    {
        string filepath = dirPath + "preferences.json";

        MTY_JSON* json = MTY_JSONObjCreate();

        MTY_JSONObjSetUInt(json, "audioInputDevice", preferences.audioInputDevice);
        MTY_JSONObjSetUInt(json, "audioOutputDevice", preferences.audioOutputDevice);
        MTY_JSONObjSetUInt(json, "micFrequency", preferences.micFrequency);
        MTY_JSONObjSetUInt(json, "micVolume", preferences.micVolume);
        MTY_JSONObjSetBool(json, "micEnabled", preferences.micEnabled);
        MTY_JSONObjSetUInt(json, "speakersFrequency", preferences.speakersFrequency);
        MTY_JSONObjSetUInt(json, "speakersVolume", preferences.speakersVolume);
        MTY_JSONObjSetBool(json, "speakersEnabled", preferences.speakersEnabled);
        MTY_JSONObjSetUInt(json, "monitor", preferences.monitor);
        MTY_JSONObjSetString(json, "roomName", preferences.roomName.c_str());
        MTY_JSONObjSetString(json, "gameID", preferences.gameID.c_str());
        MTY_JSONObjSetString(json, "secret", preferences.secret.c_str());
        MTY_JSONObjSetUInt(json, "guestCount", preferences.guestCount);
        MTY_JSONObjSetBool(json, "publicRoom", preferences.publicRoom);
        MTY_JSONObjSetInt(json, "windowX", preferences.windowX);
        MTY_JSONObjSetInt(json, "windowY", preferences.windowY);
        MTY_JSONObjSetUInt(json, "windowW", preferences.windowW);
        MTY_JSONObjSetUInt(json, "windowH", preferences.windowH);
        MTY_JSONObjSetUInt(json, "fps", preferences.fps);
        MTY_JSONObjSetUInt(json, "bandwidth", preferences.bandwidth);
        MTY_JSONObjSetUInt(json, "monitor", preferences.monitor);
        MTY_JSONObjSetUInt(json, "adapter", preferences.adapter);
        MTY_JSONObjSetUInt(json, "xboxPuppetCount", preferences.xboxPuppetCount);
        MTY_JSONObjSetUInt(json, "ds4PuppetCount", preferences.ds4PuppetCount);
        MTY_JSONObjSetBool(json, "basicVersion", preferences.basicVersion);
        MTY_JSONObjSetBool(json, "disableMicrophone", preferences.disableMicrophone);
        MTY_JSONObjSetBool(json, "disableGuideButton", preferences.disableGuideButton);
        MTY_JSONObjSetBool(json, "disableKeyboard", preferences.disableKeyboard);
        MTY_JSONObjSetString(json, "websocketURI", preferences.websocketURI.c_str());
        MTY_JSONObjSetString(json, "websocketPassword", preferences.websocketPassword.c_str());
		MTY_JSONObjSetBool(json, "showHostSettings", preferences.showHostSettings);
		MTY_JSONObjSetBool(json, "showChat", preferences.showChat);
        MTY_JSONObjSetBool(json, "showGuests", preferences.showGuests);
		MTY_JSONObjSetBool(json, "showLog", preferences.showLog);
        MTY_JSONObjSetBool(json, "showGamepads", preferences.showGamepads);
        MTY_JSONObjSetBool(json, "showMasterOfPuppets", preferences.showMasterOfPuppets);
		MTY_JSONObjSetBool(json, "showSettings", preferences.showSettings);
		MTY_JSONObjSetBool(json, "showButtonLock", preferences.showButtonLock);
		MTY_JSONObjSetBool(json, "showLibrary", preferences.showLibrary);
		MTY_JSONObjSetBool(json, "showHotseat", preferences.showHotseat);
        MTY_JSONObjSetBool(json, "showAudio", preferences.showAudio);
        MTY_JSONObjSetBool(json, "showVideo", preferences.showVideo);
        MTY_JSONObjSetBool(json, "showThumbs", preferences.showThumbs);
        MTY_JSONObjSetBool(json, "lockedGamepadLeftTrigger", preferences.lockedGamepadLeftTrigger);
        MTY_JSONObjSetBool(json, "lockedGamepadRightTrigger", preferences.lockedGamepadRightTrigger);
        MTY_JSONObjSetBool(json, "lockedGamepadLX", preferences.lockedGamepadLX);
        MTY_JSONObjSetBool(json, "lockedGamepadLY", preferences.lockedGamepadLY);
        MTY_JSONObjSetBool(json, "lockedGamepadRX", preferences.lockedGamepadRX);
        MTY_JSONObjSetBool(json, "lockedGamepadRY", preferences.lockedGamepadRY);
        MTY_JSONObjSetUInt(json, "lockedGamepadButtons", preferences.lockedGamepadButtons);
        MTY_JSONObjSetUInt(json, "theme", preferences.theme);
        MTY_JSONObjSetString(json, "discord", preferences.discord.c_str());

        // Hotseat
        MTY_JSONObjSetBool(json, "hotseat", preferences.hotseat);
        MTY_JSONObjSetBool(json, "hotseatSlotMatch", preferences.hotseatSlotMatch);
        MTY_JSONObjSetUInt(json, "hotseatSeats", preferences.hotseatSeats);
        MTY_JSONObjSetUInt(json, "hotseatTime", preferences.hotseatTime);
        MTY_JSONObjSetBool(json, "kioskMode", preferences.kioskMode);
        MTY_JSONObjSetBool(json, "hotseatAFK", preferences.hotseatAFK);
        MTY_JSONObjSetUInt(json, "hotseatAFKTime", preferences.hotseatAFKTime);
        MTY_JSONObjSetBool(json, "hotseatPause", preferences.hotseatPause);

        // Kiosk Mode
        MTY_JSONObjSetString(json, "kioskApplication", preferences.kioskApplication.c_str());
        MTY_JSONObjSetString(json, "kioskParameters", preferences.kioskParameters.c_str());

        // Chat
        MTY_JSONObjSetString(json, "chatbot", preferences.chatbot.c_str());
        MTY_JSONObjSetUInt(json, "muteTime", preferences.muteTime);
        MTY_JSONObjSetBool(json, "autoMute", preferences.autoMute);
        MTY_JSONObjSetUInt(json, "autoMuteTime", preferences.autoMuteTime);
        MTY_JSONObjSetBool(json, "saveLog", preferences.saveLog);
        MTY_JSONObjSetString(json, "welcomeMessage", preferences.welcomeMessage.c_str());

        // Tournament
        MTY_JSONObjSetBool(json, "leaderboardEnabled", preferences.leaderboardEnabled);

        // Overlay
        MTY_JSONObjSetBool(json, "overlayShow", preferences.overlayShow);
        MTY_JSONObjSetBool(json, "overlayHotseat", preferences.overlayHotseat);

        // VIP
		MTY_JSONObjSetBool(json, "vipBB", preferences.vipBB);
		MTY_JSONObjSetBool(json, "vipTournament", preferences.vipTournament);

        // Hosting
        MTY_JSONObjSetUInt(json, "selectedGame", preferences.selectedGame);
        MTY_JSONObjSetString(json, "prependRegion", preferences.prependRegion.c_str());
        MTY_JSONObjSetBool(json, "prependPingLimit", preferences.prependPingLimit);

        MTY_JSONObjSetBool(json, "firstStartup", preferences.firstStartup);

        MTY_JSONWriteFile(filepath.c_str(), json);
        MTY_JSONDestroy(&json);

        return true;
    }

    return false;
}

bool MetadataCache::savePreferences()
{
    preferences.chatbotName = "[" + preferences.chatbot + "]";
    return savePreferences(preferences);
}

vector<GuestData> MetadataCache::loadBannedUsers()
{
    vector<GuestData> result;

    string dirPath = getUserDir();
    if (!dirPath.empty())
    {
        string filepath = dirPath + "banned.json";

        if (MTY_FileExists(filepath.c_str()))
        {
            MTY_JSON* json = MTY_JSONReadFile(filepath.c_str());
            uint32_t size = MTY_JSONGetLength(json);

            for (size_t i = 0; i < size; i++)
            {
                const MTY_JSON* guest = MTY_JSONArrayGetItem(json, (uint32_t)i);

                char name[128] = "";
                uint32_t userID = 0;
                char reason[128] = "";
                bool nameSuccess = MTY_JSONObjGetString(guest, "name", name, 128);
                bool userIDSuccess = MTY_JSONObjGetUInt(guest, "userID", &userID);
                bool reasonSuccess = MTY_JSONObjGetString(guest, "reason", reason, 128);

                if (nameSuccess && userIDSuccess)
                {
                    if (reasonSuccess)
                    {
                        result.push_back(GuestData(name, userID, reason));
                    }
                    else {
                        result.push_back(GuestData(name, userID, ""));
                    }
                }
            }

            std::sort(result.begin(), result.end(), [](const GuestData a, const GuestData b) {
                return a.userID < b.userID;
                });

            MTY_JSONDestroy(&json);
        }
    }

    return result;
}

bool MetadataCache::saveBannedUsers(vector<GuestData> guests)
{
    string dirPath = getUserDir();

    if (!dirPath.empty())
    {
        string filepath = dirPath + "banned.json";

        MTY_JSON* json = MTY_JSONArrayCreate();

        vector<GuestData>::iterator gi = guests.begin();
        for (; gi != guests.end(); ++gi)
        {
            MTY_JSON* guest = MTY_JSONObjCreate();

            MTY_JSONObjSetString(guest, "name", (*gi).name.c_str());
            MTY_JSONObjSetUInt(guest, "userID", (*gi).userID);
            MTY_JSONObjSetString(guest, "reason", (*gi).reason.c_str());
            MTY_JSONArrayAppendItem(json, guest);
        }

        MTY_JSONWriteFile(filepath.c_str(), json);
        MTY_JSONDestroy(&json);

        return true;
    }

    return false;
}

vector<GuestData> MetadataCache::loadModdedUsers() {

    vector<GuestData> result;

    string dirPath = getUserDir();
    if (!dirPath.empty())
    {
        string filepath = dirPath + "mods.json";

        if (MTY_FileExists(filepath.c_str()))
        {
            MTY_JSON* json = MTY_JSONReadFile(filepath.c_str());
            uint32_t size = MTY_JSONGetLength(json);

            for (size_t i = 0; i < size; i++)
            {
                const MTY_JSON* guest = MTY_JSONArrayGetItem(json, (uint32_t)i);

                char name[128] = "";
                uint32_t userID = 0;
                bool nameSuccess = MTY_JSONObjGetString(guest, "name", name, 128);
                bool userIDSuccess = MTY_JSONObjGetUInt(guest, "userID", &userID);

                if (nameSuccess && userIDSuccess)
                {
                    result.push_back(GuestData(name, userID));
                }
            }

            std::sort(result.begin(), result.end(), [](const GuestData a, const GuestData b) {
                return a.userID < b.userID;
                });

            MTY_JSONDestroy(&json);
        }
    }

    return result;

}

bool MetadataCache::saveModdedUsers(vector<GuestData> guests)
{

    string dirPath = getUserDir();

    if (!dirPath.empty())
    {
        string filepath = dirPath + "mods.json";

        MTY_JSON* json = MTY_JSONArrayCreate();

        vector<GuestData>::iterator gi = guests.begin();
        for (; gi != guests.end(); ++gi)
        {
            MTY_JSON* guest = MTY_JSONObjCreate();

            MTY_JSONObjSetString(guest, "name", (*gi).name.c_str());
            MTY_JSONObjSetUInt(guest, "userID", (*gi).userID);
            MTY_JSONArrayAppendItem(json, guest);
        }

        MTY_JSONWriteFile(filepath.c_str(), json);
        MTY_JSONDestroy(&json);

        return true;
    }

    return false;
}

vector<GuestData> MetadataCache::loadVIPUsers() {

    vector<GuestData> result;

    string dirPath = getUserDir();
    if (!dirPath.empty())
    {
        string filepath = dirPath + "vip.json";

        if (MTY_FileExists(filepath.c_str()))
        {
            MTY_JSON* json = MTY_JSONReadFile(filepath.c_str());
            uint32_t size = MTY_JSONGetLength(json);

            for (size_t i = 0; i < size; i++)
            {
                const MTY_JSON* guest = MTY_JSONArrayGetItem(json, (uint32_t)i);

                char name[128] = "";
                uint32_t userID = 0;
                bool nameSuccess = MTY_JSONObjGetString(guest, "name", name, 128);
                bool userIDSuccess = MTY_JSONObjGetUInt(guest, "userID", &userID);

                if (nameSuccess && userIDSuccess)
                {
                    result.push_back(GuestData(name, userID));
                }
            }

            std::sort(result.begin(), result.end(), [](const GuestData a, const GuestData b) {
                return a.userID < b.userID;
                });

            MTY_JSONDestroy(&json);
        }
    }

    return result;

}

bool MetadataCache::saveVIPUsers(vector<GuestData> guests)
{

    string dirPath = getUserDir();

    if (!dirPath.empty())
    {
        string filepath = dirPath + "vip.json";

        MTY_JSON* json = MTY_JSONArrayCreate();

        vector<GuestData>::iterator gi = guests.begin();
        for (; gi != guests.end(); ++gi)
        {
            MTY_JSON* guest = MTY_JSONObjCreate();

            MTY_JSONObjSetString(guest, "name", (*gi).name.c_str());
            MTY_JSONObjSetUInt(guest, "userID", (*gi).userID);
            MTY_JSONArrayAppendItem(json, guest);
        }

        MTY_JSONWriteFile(filepath.c_str(), json);
        MTY_JSONDestroy(&json);

        return true;
    }

    return false;
}

vector<GameData> MetadataCache::loadGamesList() {

    vector<GameData> result;

    string dirPath = getUserDir();
    if (!dirPath.empty())
    {
        string filepath = dirPath + "games.json";

        if (MTY_FileExists(filepath.c_str()))
        {
            MTY_JSON* json = MTY_JSONReadFile(filepath.c_str());
            uint32_t size = MTY_JSONGetLength(json);

            for (size_t i = 0; i < size; i++)
            {
                const MTY_JSON* game = MTY_JSONArrayGetItem(json, (uint32_t)i);

                uint32_t itemID = 0;
                char name[128] = "";
                char path[256] = "";
                char parameters[256] = "";
                char thumbnailPath[256] = "";
                char gameID[256] = "";
				bool kiosk = false;
				bool hotseat = false;
				uint32_t seats = 0;

				bool itemIDSuccess = MTY_JSONObjGetUInt(game, "itemID", &itemID);
                bool nameSuccess = MTY_JSONObjGetString(game, "name", name, 128);
                bool pathSuccess = MTY_JSONObjGetString(game, "path", path, 256);
                bool paramSuccess = MTY_JSONObjGetString(game, "parameters", parameters, 256);

				if (itemIDSuccess && nameSuccess && pathSuccess && paramSuccess)
                {
                    result.push_back(GameData(itemID, name, path, parameters));
                }
            }

            /*std::sort(result.begin(), result.end(), [](const GameData a, const GameData b) {
                return a.gameID < b.gameID;
            });*/

            MTY_JSONDestroy(&json);
        }
    }

    return result;

}

bool MetadataCache::saveGamesList(vector<GameData> games)
{

    string dirPath = getUserDir();

    if (!dirPath.empty())
    {
        string filepath = dirPath + "games.json";

        MTY_JSON* json = MTY_JSONArrayCreate();

        vector<GameData>::iterator gi = games.begin();
        for (; gi != games.end(); ++gi)
        {
            MTY_JSON* game = MTY_JSONObjCreate();

			MTY_JSONObjSetUInt(game, "itemID", (*gi).itemID);
            MTY_JSONObjSetString(game, "name", (*gi).name.c_str());
            MTY_JSONObjSetString(game, "path", (*gi).path.c_str());
            MTY_JSONObjSetString(game, "parameters", (*gi).parameters.c_str());

            MTY_JSONArrayAppendItem(json, game);
        }

        MTY_JSONWriteFile(filepath.c_str(), json);
        MTY_JSONDestroy(&json);

        return true;
    }

    return false;
}

vector<GuestTier> MetadataCache::loadGuestTiers()
{
    vector<GuestTier> result;

    string dirPath = getUserDir();
    if (!dirPath.empty())
    {
        string filepath = dirPath + "tiers.json";

        if (MTY_FileExists(filepath.c_str()))
        {
            MTY_JSON* json = MTY_JSONReadFile(filepath.c_str());
            uint32_t size = MTY_JSONGetLength(json);

            for (size_t i = 0; i < size; i++)
            {
                const MTY_JSON* guest = MTY_JSONArrayGetItem(json, (uint32_t)i);

                char name[128] = "";
                uint32_t userID, tier = 0;
                bool tierSuccess = MTY_JSONObjGetUInt(guest, "tier", &tier);
                bool userIDSuccess = MTY_JSONObjGetUInt(guest, "userID", &userID);

                if (tierSuccess && userIDSuccess)
                {
                    result.push_back(GuestTier(userID, (Tier)tier));
                }
            }

            std::sort(result.begin(), result.end(), [](const GuestTier a, const GuestTier b) {
                return a.userID < b.userID;
                });

            MTY_JSONDestroy(&json);
        }
    }

    return result;
}

bool MetadataCache::saveGuestTiers(vector<GuestTier> guestTiers)
{
    string dirPath = getUserDir();

    if (!dirPath.empty())
    {
        string filepath = dirPath + "tiers.json";

        MTY_JSON* json = MTY_JSONArrayCreate();

        vector<GuestTier>::iterator gi = guestTiers.begin();
        for (; gi != guestTiers.end(); ++gi)
        {
            MTY_JSON* guest = MTY_JSONObjCreate();

            MTY_JSONObjSetUInt(guest, "tier", (uint32_t)(*gi).tier);
            MTY_JSONObjSetUInt(guest, "userID", (*gi).userID);
            MTY_JSONArrayAppendItem(json, guest);
        }

        MTY_JSONWriteFile(filepath.c_str(), json);
        MTY_JSONDestroy(&json);

        return true;
    }

    return false;
}

bool MetadataCache::saveTheme(int theme) {

    return true;

}

std::string GetCurrentDirectory()
{
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string::size_type pos = std::string(buffer).find_last_of("\\/");
    return std::string(buffer).substr(0, pos);
}

string MetadataCache::getUserDir()
{
    string dir = GetCurrentDirectory();
    string appDir = "\\SmashSodaTwo\\";
    if (MTY_FileExists((dir + "\\portable.txt").c_str()))
    {
        string dirPath = dir + appDir;
        bool isDirOk = false;
        if (!MTY_FileExists(dirPath.c_str()))
        {
            if (MTY_Mkdir(dirPath.c_str()))
            {
                isDirOk = true;
            }
        }
        else
        {
            isDirOk = true;
        }
        if (isDirOk)
        {
            return dirPath;
        }
    }
    else
    {
        TCHAR tAppdata[1024];
        if (SUCCEEDED(SHGetFolderPath(nullptr, CSIDL_APPDATA, NULL, 0, tAppdata)))
        {
            wstring wAppdata(tAppdata);
            string appdata(wAppdata.begin(), wAppdata.end());
            string dirPath = appdata + appDir;

            bool isDirOk = false;

            if (!MTY_FileExists(dirPath.c_str()))
            {
                if (MTY_Mkdir(dirPath.c_str()))
                {
                    isDirOk = true;
                }
            }
            else
            {
                isDirOk = true;
            }

            if (isDirOk)
            {
                return dirPath;
            }
        }
    }

    return string();
}

bool MetadataCache::isSpectating(uint32_t guestID) {
    if (MetadataCache::preferences.activeGuests.empty() == false) {
        for (int i = MetadataCache::preferences.activeGuests.size() - 1; i >= 0; i--) {
            if (MetadataCache::preferences.activeGuests.at(i).userID == guestID) {
                return false;
            }
        }
    }
    return true;
}

bool MetadataCache::addActiveGuest(Guest guest) {

	preferences.activeGuests.push_back(GuestData(guest.name, guest.userID));

    return true;

}

bool MetadataCache::removeActiveGuest(Guest guest) {

    for (int i = MetadataCache::preferences.activeGuests.size() - 1; i >= 0; i--) {
        if (MetadataCache::preferences.activeGuests.at(i).userID == guest.userID) {
            MetadataCache::preferences.activeGuests.erase(MetadataCache::preferences.activeGuests.begin() + i);
            MetadataCache::preferences.activeGuests.shrink_to_fit();
            return true;
        }
    }

}