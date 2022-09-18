#include "MetadataCache.h"

// This is not ideal, especially in an open source environment.
// I'm using these values just as placeholders until I find an
// actual solution. You should change them in your build.
string MetadataCache::_key = "ParsecSodaKey***";
string MetadataCache::_nonce = "ParsecSoda**";
MetadataCache::Preferences MetadataCache::preferences = MetadataCache::Preferences();
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
            MTY_JSON *json = MTY_JSONReadFile(filepath.c_str());
            char roomName[256] = "", gameID[72] = "", secret[32] = "";
            char websocketURI[50] = "";
            char websocketPassword[32] = "";
            char discord[256] = "";

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
                preferences.publicRoom = false;
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
            if (!MTY_JSONObjGetBool(json, "disableGuideButton", &preferences.disableGuideButton)) preferences.disableGuideButton = false;
            if (!MTY_JSONObjGetBool(json, "disableKeyboard", &preferences.disableKeyboard)) preferences.disableKeyboard = false;

            if (MTY_JSONObjGetString(json, "websocketURI", websocketURI, 50)) preferences.websocketURI = websocketURI;
            else preferences.websocketURI = "ws://127.0.0.1:9002";

            if (MTY_JSONObjGetString(json, "websocketPassword", websocketPassword, 32)) preferences.websocketPassword = websocketPassword;
            else preferences.websocketPassword = "";

            if (!MTY_JSONObjGetBool(json, "showMasterOfPuppets", &preferences.showMasterOfPuppets)) preferences.showMasterOfPuppets = false;
            if (!MTY_JSONObjGetBool(json, "showAudio", &preferences.showAudio)) preferences.showAudio = false;
            if (!MTY_JSONObjGetBool(json, "showVideo", &preferences.showVideo)) preferences.showVideo = false;
            if (!MTY_JSONObjGetBool(json, "showThumbs", &preferences.showThumbs)) preferences.showThumbs = false;
            if (!MTY_JSONObjGetBool(json, "showWebSocket", &preferences.showWebSocket)) preferences.showWebSocket = false;
            if (!MTY_JSONObjGetBool(json, "latencyLimitEnabled", &preferences.latencyLimitEnabled)) preferences.latencyLimitEnabled = false;
            if (!MTY_JSONObjGetUInt(json, "latencyLimitValue", &preferences.latencyLimitValue)) preferences.latencyLimitValue = 200;
            if (!MTY_JSONObjGetBool(json, "lockedGamepadLeftTrigger", &preferences.lockedGamepadLeftTrigger)) preferences.lockedGamepadLeftTrigger = false;
            if (!MTY_JSONObjGetBool(json, "lockedGamepadRightTrigger", &preferences.lockedGamepadRightTrigger)) preferences.lockedGamepadRightTrigger = false;
            if (!MTY_JSONObjGetBool(json, "lockedGamepadLX", &preferences.lockedGamepadLX)) preferences.lockedGamepadLX = false;
            if (!MTY_JSONObjGetBool(json, "lockedGamepadLY", &preferences.lockedGamepadLY)) preferences.lockedGamepadLY = false;
            if (!MTY_JSONObjGetBool(json, "lockedGamepadRX", &preferences.lockedGamepadRX)) preferences.lockedGamepadRX = false;
            if (!MTY_JSONObjGetBool(json, "lockedGamepadRY", &preferences.lockedGamepadRY)) preferences.lockedGamepadRY = false;
            if (!MTY_JSONObjGetUInt(json, "lockedGamepadButtons", &preferences.lockedGamepadButtons)) preferences.lockedGamepadButtons = 0;
            if (!MTY_JSONObjGetUInt(json, "theme", &preferences.theme)) preferences.theme = 0;

            if (MTY_JSONObjGetString(json, "discord", discord, 256)) preferences.discord = discord;
            else preferences.discord = "";

            if (!MTY_JSONObjGetBool(json, "hotseat", &preferences.hotseat)) preferences.latencyLimitEnabled = false;
            if (!MTY_JSONObjGetUInt(json, "hotseatTime", &preferences.hotseatTime)) preferences.hotseatTime = 15;

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

        MTY_JSON *json = MTY_JSONObjCreate();
            
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
        MTY_JSONObjSetBool(json, "showMasterOfPuppets", preferences.showMasterOfPuppets);
        MTY_JSONObjSetBool(json, "showAudio", preferences.showAudio);
        MTY_JSONObjSetBool(json, "showVideo", preferences.showVideo);
        MTY_JSONObjSetBool(json, "showThumbs", preferences.showThumbs);
        MTY_JSONObjSetBool(json, "showWebSocket", preferences.showWebSocket);
        MTY_JSONObjSetBool(json, "latencyLimitEnabled", preferences.latencyLimitEnabled);
        MTY_JSONObjSetUInt(json, "latencyLimitValue", preferences.latencyLimitValue);
        MTY_JSONObjSetBool(json, "lockedGamepadLeftTrigger", preferences.lockedGamepadLeftTrigger);
        MTY_JSONObjSetBool(json, "lockedGamepadRightTrigger", preferences.lockedGamepadRightTrigger);
        MTY_JSONObjSetBool(json, "lockedGamepadLX", preferences.lockedGamepadLX);
        MTY_JSONObjSetBool(json, "lockedGamepadLY", preferences.lockedGamepadLY);
        MTY_JSONObjSetBool(json, "lockedGamepadRX", preferences.lockedGamepadRX);
        MTY_JSONObjSetBool(json, "lockedGamepadRY", preferences.lockedGamepadRY);
        MTY_JSONObjSetUInt(json, "lockedGamepadButtons", preferences.lockedGamepadButtons);
        MTY_JSONObjSetUInt(json, "theme", preferences.theme);
        MTY_JSONObjSetString(json, "discord", preferences.discord.c_str());
        MTY_JSONObjSetBool(json, "hotseat", preferences.hotseat);
        MTY_JSONObjSetUInt(json, "hotseatTime", preferences.hotseatTime);

        MTY_JSONWriteFile(filepath.c_str(), json);
        MTY_JSONDestroy(&json);
            
        return true;
    }

    return false;
}

bool MetadataCache::savePreferences()
{
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
                
                char name [128] = "";
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

vector<Thumbnail> MetadataCache::loadThumbnails()
{
    _mutex.lock();

    vector<Thumbnail> result;

    string dirPath = getUserDir();
    if (!dirPath.empty())
    {
        string filepath = dirPath + "thumbnails.json";

        if (MTY_FileExists(filepath.c_str()))
        {
            size_t size;
            const char* encryptedContent;
            encryptedContent = (char*)MTY_ReadFile(filepath.c_str(), &size);

            char *originalText = new char[size + 100];
            MTY_AESGCM *ctx = MTY_AESGCMCreate(_key.c_str());
            char tag[16] = "ParsecSodaTag**";

            MTY_AESGCMDecrypt(ctx, _nonce.c_str(), encryptedContent, size + 100, tag, (void*)originalText);
            MTY_JSON* json = MTY_JSONParse(originalText);
            uint32_t thumbnailCount = MTY_JSONGetLength(json);

            for (size_t i = 0; i < thumbnailCount; i++)
            {
                const MTY_JSON* guest = MTY_JSONArrayGetItem(json, (uint32_t)i);

                char name[256] = "", gameId[64] = "";

                bool success =
                    MTY_JSONObjGetString(guest, "gameId", gameId, 64) &&
                    MTY_JSONObjGetString(guest, "name", name, 256);

                if (success)
                {
                    result.push_back(Thumbnail(gameId, name, true));
                }
            }
            delete[] originalText;

            //std::sort(result.begin(), result.end(), [](const Thumbnail a, const Thumbnail b) {
            //    int compare = Stringer::toLower(a.name).compare(Stringer::toLower(b.name));
            //    return compare;
            //});

            MTY_JSONDestroy(&json);
            MTY_AESGCMDestroy(&ctx);
        }
    }

    _mutex.unlock();
    return result;
}

bool MetadataCache::saveThumbnails(vector<Thumbnail> thumbnails)
{
    _mutex.lock();

    bool result = false;
    string dirPath = getUserDir();

    if (!dirPath.empty())
    {
        string filepath = dirPath + "thumbnails.json";

        MTY_JSON* json = MTY_JSONArrayCreate();

        vector<Thumbnail>::iterator it = thumbnails.begin();
        for (; it != thumbnails.end(); ++it)
        {
            if ((*it).saved)
            {
                MTY_JSON* thumb = MTY_JSONObjCreate();

                MTY_JSONObjSetString(thumb, "gameId", (*it).gameId.c_str());
                MTY_JSONObjSetString(thumb, "name", (*it).name.c_str());
                MTY_JSONArrayAppendItem(json, thumb);
            }
        }

        string jsonStr = MTY_JSONSerialize(json);
        char *encryptedJson = new char[jsonStr.size() + 100];
        char tag[16] = "ParsecSodaTag**";
        MTY_AESGCM* ctx = MTY_AESGCMCreate(_key.c_str());
        if (MTY_AESGCMEncrypt(ctx, _nonce.c_str(), jsonStr.c_str(), jsonStr.size() + 100, tag, encryptedJson))
        {
            if (MTY_WriteFile(filepath.c_str(), encryptedJson, jsonStr.size()))
            {
                result = true;
            }
        }
        delete[] encryptedJson;

        MTY_JSONDestroy(&json);
        MTY_AESGCMDestroy(&ctx);
    }

    _mutex.unlock();
    return result;
}

bool MetadataCache::saveTheme(int theme) {

    preferences.theme = theme;
    MetadataCache::savePreferences();

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
    string appDir = "\\ParsecSodaV\\";
    if (MTY_FileExists( (dir+"\\portable.txt").c_str()  ))
    {
        string dirPath = dir+appDir;
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