#include "Config.h"

Config Config::cfg = Config::Config();

/// <summary>
/// Loads the config file.
/// </summary>
void Config::Load() {

	// Get the config path
	string configPath = GetConfigPath() + "\\config.json";
	if (MTY_FileExists(configPath.c_str())) {

		try {

			size_t size;
			void* data = MTY_ReadFile(configPath.c_str(), &size);

			// Parse the json
			json j = json::parse((char*)data);

			// Set General properties
			cfg.general.theme = setValue(cfg.general.theme, j["General"]["theme"].get<unsigned int>());
			cfg.general.saveLog = setValue(cfg.general.saveLog, j["General"]["saveLog"].get<bool>());
			cfg.general.flashWindow = setValue(cfg.general.flashWindow, j["General"]["flashWindow"].get<bool>());

			// Set Audio properties
			cfg.audio.inputDevice = setValue(cfg.audio.inputDevice, j["Audio"]["inputDevice"].get<unsigned int>());
			cfg.audio.outputDevice = setValue(cfg.audio.outputDevice, j["Audio"]["outputDevice"].get<unsigned int>());
			cfg.audio.micFrequency = setValue(cfg.audio.micFrequency, j["Audio"]["micFrequency"].get<unsigned int>());
			cfg.audio.micVolume = setValue(cfg.audio.micVolume, j["Audio"]["micVolume"].get<unsigned int>());
			cfg.audio.micEnabled = setValue(cfg.audio.micEnabled, j["Audio"]["micEnabled"].get<bool>());
			cfg.audio.speakersFrequency = setValue(cfg.audio.speakersFrequency, j["Audio"]["speakersFrequency"].get<unsigned int>());
			cfg.audio.speakersVolume = setValue(cfg.audio.speakersVolume, j["Audio"]["speakersVolume"].get<unsigned int>());
			cfg.audio.speakersEnabled = setValue(cfg.audio.speakersEnabled, j["Audio"]["speakersEnabled"].get<bool>());
			cfg.audio.sfxEnabled = setValue(cfg.audio.sfxEnabled, j["Audio"]["sfxEnabled"].get<bool>());

			// Set Video properties
			cfg.video.monitor = setValue(cfg.video.monitor, j["Video"]["monitor"].get<unsigned int>());
			cfg.video.adapter = setValue(cfg.video.adapter, j["Video"]["adapter"].get<unsigned int>());
			cfg.video.windowX = setValue(cfg.video.windowX, j["Video"]["windowX"].get<int>());
			cfg.video.windowY = setValue(cfg.video.windowY, j["Video"]["windowY"].get<int>());
			cfg.video.windowW = setValue(cfg.video.windowW, j["Video"]["windowW"].get<int>());
			cfg.video.windowH = setValue(cfg.video.windowH, j["Video"]["windowH"].get<int>());
			cfg.video.fps = setValue(cfg.video.fps, j["Video"]["fps"].get<unsigned int>());
			cfg.video.bandwidth = setValue(cfg.video.bandwidth, j["Video"]["bandwidth"].get<unsigned int>());

			// Set Input properties
			cfg.input.autoIndex = setValue(cfg.input.autoIndex, j["Input"]["autoIndex"].get<bool>());
			cfg.input.disableKeyboard = setValue(cfg.input.disableKeyboard, j["Input"]["disableKeyboard"].get<bool>());
			cfg.input.disableGuideButton = setValue(cfg.input.disableGuideButton, j["Input"]["disableGuideButton"].get<bool>());
			cfg.input.xboxPuppetCount = setValue(cfg.input.xboxPuppetCount, j["Input"]["xboxPuppetCount"].get<unsigned int>());
			cfg.input.ds4PuppetCount = setValue(cfg.input.ds4PuppetCount, j["Input"]["ds4PuppetCount"].get<unsigned int>());
			cfg.input.lockedPads = setValue(cfg.input.lockedPads, j["Input"]["lockedPads"].get<bool>());
			cfg.input.lockedGamepadLeftTrigger = setValue(cfg.input.lockedGamepadLeftTrigger, j["Input"]["lockedGamepadLeftTrigger"].get<bool>());
			cfg.input.lockedGamepadRightTrigger = setValue(cfg.input.lockedGamepadRightTrigger, j["Input"]["lockedGamepadRightTrigger"].get<bool>());
			cfg.input.lockedGamepadLX = setValue(cfg.input.lockedGamepadLX, j["Input"]["lockedGamepadLX"].get<bool>());
			cfg.input.lockedGamepadLY = setValue(cfg.input.lockedGamepadLY, j["Input"]["lockedGamepadLY"].get<bool>());
			cfg.input.lockedGamepadRX = setValue(cfg.input.lockedGamepadRX, j["Input"]["lockedGamepadRX"].get<bool>());
			cfg.input.lockedGamepadRY = setValue(cfg.input.lockedGamepadRY, j["Input"]["lockedGamepadRY"].get<bool>());
			cfg.input.lockedGamepadButtons = setValue(cfg.input.lockedGamepadButtons, j["Input"]["lockedGamepadButtons"].get<unsigned int>());

			// Set Room properties
			cfg.room.title = setValue(cfg.room.title, j["Room"]["title"].get<string>());
			cfg.room.details = setValue(cfg.room.details, j["Room"]["details"].get<string>());
			cfg.room.theme = setValue(cfg.room.theme, j["Room"]["theme"].get<string>());
			cfg.room.game = setValue(cfg.room.game, j["Room"]["game"].get<string>());
			cfg.room.privateRoom = setValue(cfg.room.privateRoom, j["Room"]["privateRoom"].get<bool>());
			cfg.room.guestLimit = setValue(cfg.room.guestLimit, j["Room"]["guestLimit"].get<unsigned int>());
			cfg.room.isValid = setValue(cfg.room.isValid, j["Room"]["isValid"].get<bool>());
			cfg.room.secret = setValue(cfg.room.secret, j["Room"]["secret"].get<string>());

			cfg.room.latencyLimit = setValue(cfg.room.latencyLimit, j["Room"]["latencyLimit"].get<bool>());
			cfg.room.latencyLimitThreshold = setValue(cfg.room.latencyLimitThreshold, j["Room"]["latencyLimitThreshold"].get<unsigned int>());

			// Set Chat properties
			cfg.chat.discord = setValue(cfg.chat.discord, j["Chat"]["discord"].get<string>());
			cfg.chat.chatbot = setValue(cfg.chat.chatbot, j["Chat"]["chatbot"].get<string>());
			cfg.chat.chatbotName = setValue(cfg.chat.chatbotName, j["Chat"]["chatbotName"].get<string>());
			cfg.chat.muteTime = setValue(cfg.chat.muteTime, j["Chat"]["muteTime"].get<unsigned int>());
			cfg.chat.autoMute = setValue(cfg.chat.autoMute, j["Chat"]["autoMute"].get<bool>());
			cfg.chat.autoMuteTime = setValue(cfg.chat.autoMuteTime, j["Chat"]["autoMuteTime"].get<unsigned int>());
			cfg.chat.welcomeMessage = setValue(cfg.chat.welcomeMessage, j["Chat"]["welcomeMessage"].get<string>());
			cfg.chat.bonkEnabled = setValue(cfg.chat.bonkEnabled, j["Chat"]["bonkEnabled"].get<bool>());
			cfg.chat.hostBonkProof = setValue(cfg.chat.hostBonkProof, j["Chat"]["hostBonkProof"].get<bool>());

			// Set Widgets properties
			cfg.widgets.host = setValue(cfg.widgets.host, j["Widgets"]["host"].get<bool>());
			cfg.widgets.guests = setValue(cfg.widgets.guests, j["Widgets"]["guests"].get<bool>());
			cfg.widgets.gamepads = setValue(cfg.widgets.gamepads, j["Widgets"]["gamepads"].get<bool>());
			cfg.widgets.chat = setValue(cfg.widgets.chat, j["Widgets"]["chat"].get<bool>());
			cfg.widgets.log = setValue(cfg.widgets.log, j["Widgets"]["log"].get<bool>());
			cfg.widgets.settings = setValue(cfg.widgets.settings, j["Widgets"]["settings"].get<bool>());
			cfg.widgets.buttonLock = setValue(cfg.widgets.buttonLock, j["Widgets"]["buttonLock"].get<bool>());
			cfg.widgets.library = setValue(cfg.widgets.library, j["Widgets"]["library"].get<bool>());
			cfg.widgets.hotseat = setValue(cfg.widgets.hotseat, j["Widgets"]["hotseat"].get<bool>());
			cfg.widgets.masterOfPuppets = setValue(cfg.widgets.masterOfPuppets, j["Widgets"]["masterOfPuppets"].get<bool>());
			cfg.widgets.audio = setValue(cfg.widgets.audio, j["Widgets"]["audio"].get<bool>());
			cfg.widgets.video = setValue(cfg.widgets.video, j["Widgets"]["video"].get<bool>());
			cfg.widgets.overlay = setValue(cfg.widgets.overlay, j["Widgets"]["overlay"].get<bool>());
			cfg.widgets.keyMapper = setValue(cfg.widgets.keyMapper, j["Widgets"]["keyMapper"].get<bool>());

			// Set Hotseat properties
			cfg.hotseat.enabled = setValue(cfg.hotseat.enabled, j["Hotseat"]["enabled"].get<bool>());
			cfg.hotseat.playTime = setValue(cfg.hotseat.playTime, j["Hotseat"]["playTime"].get<unsigned int>());
			cfg.hotseat.resetTime = setValue(cfg.hotseat.resetTime, j["Hotseat"]["resetTime"].get<unsigned int>());

			// Set KioskMode properties
			cfg.kioskMode.enabled = setValue(cfg.kioskMode.enabled, j["KioskMode"]["enabled"].get<bool>());

			// Set Overlay properties
			cfg.overlay.update = setValue(cfg.overlay.update, j["Overlay"]["update"].get<bool>());
			cfg.overlay.enabled = setValue(cfg.overlay.enabled, j["Overlay"]["enabled"].get<bool>());
			cfg.overlay.opacity = setValue(cfg.overlay.opacity, j["Overlay"]["opacity"].get<float>());
			cfg.overlay.port = setValue(cfg.overlay.port, j["Overlay"]["port"].get<string>());

			// Set Overlay::Chat properties
			cfg.overlay.chat.active = setValue(cfg.overlay.chat.active, j["Overlay"]["chat"]["active"].get<bool>());
			cfg.overlay.chat.position = setValue(cfg.overlay.chat.position, j["Overlay"]["chat"]["position"].get<string>());
			cfg.overlay.chat.showHistory = setValue(cfg.overlay.chat.showHistory, j["Overlay"]["chat"]["showHistory"].get<bool>());

			// Set Overlay::Gamepads properties
			cfg.overlay.gamepads.active = setValue(cfg.overlay.gamepads.active, j["Overlay"]["gamepads"]["active"].get<bool>());
			cfg.overlay.gamepads.position = setValue(cfg.overlay.gamepads.position, j["Overlay"]["gamepads"]["position"].get<string>());
			cfg.overlay.gamepads.showHotseatDuration = setValue(cfg.overlay.gamepads.showHotseatDuration, j["Overlay"]["gamepads"]["showHotseatDuration"].get<bool>());

			// Set Overlay::Guests properties
			cfg.overlay.guests.active = setValue(cfg.overlay.guests.active, j["Overlay"]["guests"]["active"].get<bool>());
			cfg.overlay.guests.position = setValue(cfg.overlay.guests.position, j["Overlay"]["guests"]["position"].get<string>());
			cfg.overlay.guests.showLatency = setValue(cfg.overlay.guests.showLatency, j["Overlay"]["guests"]["showLatency"].get<bool>());

			// Set Permissions properties
			cfg.permissions.guest.useBB = setValue(cfg.permissions.guest.useBB, j["Permissions"]["guest"]["useBB"].get<bool>());
			cfg.permissions.guest.useSFX = setValue(cfg.permissions.guest.useSFX, j["Permissions"]["guest"]["useSFX"].get<bool>());
			cfg.permissions.guest.changeControls = setValue(cfg.permissions.guest.changeControls, j["Permissions"]["guest"]["changeControls"].get<bool>());
			cfg.permissions.vip.useBB = setValue(cfg.permissions.vip.useBB, j["Permissions"]["vip"]["useBB"].get<bool>());
			cfg.permissions.vip.useSFX = setValue(cfg.permissions.vip.useSFX, j["Permissions"]["vip"]["useSFX"].get<bool>());
			cfg.permissions.vip.changeControls = setValue(cfg.permissions.vip.changeControls, j["Permissions"]["vip"]["changeControls"].get<bool>());
			cfg.permissions.moderator.useBB = setValue(cfg.permissions.moderator.useBB, j["Permissions"]["moderator"]["useBB"].get<bool>());
			cfg.permissions.moderator.useSFX = setValue(cfg.permissions.moderator.useSFX, j["Permissions"]["moderator"]["useSFX"].get<bool>());
			cfg.permissions.moderator.changeControls = setValue(cfg.permissions.moderator.changeControls, j["Permissions"]["moderator"]["changeControls"].get<bool>());

			// Set Arcade properties
			cfg.arcade.token = setValue(cfg.arcade.token, j["Arcade"]["token"].get<string>());
			cfg.arcade.username = setValue(cfg.arcade.username, j["Arcade"]["username"].get<string>());
			cfg.arcade.showLogin = setValue(cfg.arcade.showLogin, j["Arcade"]["showLogin"].get<bool>());

		} catch (json::exception &e) {
			// Handle exception
		}

	}

}

/// <summary>
/// Saves the config file.
/// </summary>
void Config::Save() {

	json j;

	// General
	j["General"] = {
		{"theme", cfg.general.theme},
		{"saveLog", cfg.general.saveLog},
		{"flashWindow", cfg.general.flashWindow}
	};

	// Audio
	j["Audio"] = {
		{"inputDevice", cfg.audio.inputDevice},
		{"outputDevice", cfg.audio.outputDevice},
		{"micFrequency", cfg.audio.micFrequency},
		{"micVolume", cfg.audio.micVolume},
		{"micEnabled", cfg.audio.micEnabled},
		{"speakersFrequency", cfg.audio.speakersFrequency},
		{"speakersVolume", cfg.audio.speakersVolume},
		{"speakersEnabled", cfg.audio.speakersEnabled},
		{"sfxEnabled", cfg.audio.sfxEnabled}
	};

	// Video
	j["Video"] = {
		{"monitor", cfg.video.monitor},
		{"adapter", cfg.video.adapter},
		{"windowX", cfg.video.windowX},
		{"windowY", cfg.video.windowY},
		{"windowW", cfg.video.windowW},
		{"windowH", cfg.video.windowH},
		{"fps", cfg.video.fps},
		{"bandwidth", cfg.video.bandwidth}
	};

	// Input
	j["Input"] = {
		{"autoIndex", cfg.input.autoIndex},
		{"disableKeyboard", cfg.input.disableKeyboard},
		{"disableGuideButton", cfg.input.disableGuideButton},
		{"xboxPuppetCount", cfg.input.xboxPuppetCount},
		{"ds4PuppetCount", cfg.input.ds4PuppetCount},
		{"lockedPads", cfg.input.lockedPads},
		{"lockedGamepadLeftTrigger", cfg.input.lockedGamepadLeftTrigger},
		{"lockedGamepadRightTrigger", cfg.input.lockedGamepadRightTrigger},
		{"lockedGamepadLX", cfg.input.lockedGamepadLX},
		{"lockedGamepadLY", cfg.input.lockedGamepadLY},
		{"lockedGamepadRX", cfg.input.lockedGamepadRX},
		{"lockedGamepadRY", cfg.input.lockedGamepadRY},
		{"lockedGamepadButtons", cfg.input.lockedGamepadButtons}
	};

	// Room
	j["Room"] = {
		{"title", cfg.room.title},
		{"details", cfg.room.details},
		{"theme", cfg.room.theme},
		{"game", cfg.room.game},
		{"privateRoom", cfg.room.privateRoom},
		{"guestLimit", cfg.room.guestLimit},
		{"isValid", cfg.room.isValid},
		{"secret", cfg.room.secret},
		{"latencyLimit", cfg.room.latencyLimit},
		{"latencyLimitThreshold", cfg.room.latencyLimitThreshold}
	};

	// Chat
	j["Chat"] = {
		{"discord", cfg.chat.discord},
		{"chatbot", cfg.chat.chatbot},
		{"chatbotName", cfg.chat.chatbotName},
		{"muteTime", cfg.chat.muteTime},
		{"autoMute", cfg.chat.autoMute},
		{"autoMuteTime", cfg.chat.autoMuteTime},
		{"welcomeMessage", cfg.chat.welcomeMessage},
		{"bonkEnabled", cfg.chat.bonkEnabled},
		{"hostBonkProof", cfg.chat.hostBonkProof}
	};

	// Widgets
	j["Widgets"] = {
		{"host", cfg.widgets.host},
		{"guests", cfg.widgets.guests},
		{"gamepads", cfg.widgets.gamepads},
		{"chat", cfg.widgets.chat},
		{"log", cfg.widgets.log},
		{"settings", cfg.widgets.settings},
		{"buttonLock", cfg.widgets.buttonLock},
		{"library", cfg.widgets.library},
		{"hotseat", cfg.widgets.hotseat},
		{"masterOfPuppets", cfg.widgets.masterOfPuppets},
		{"audio", cfg.widgets.audio},
		{"video", cfg.widgets.video},
		{"overlay", cfg.widgets.overlay},
		{"keyMapper", cfg.widgets.keyMapper}
	};

	// Hotseat
	j["Hotseat"] = {
		{"enabled", cfg.hotseat.enabled},
		{"playTime", cfg.hotseat.playTime},
		{"resetTime", cfg.hotseat.resetTime}
	};

	// KioskMode
	j["KioskMode"] = {
		{"enabled", cfg.kioskMode.enabled}
	};

	// Overlay
	j["Overlay"] = {
		{"update", cfg.overlay.update},
		{"enabled", cfg.overlay.enabled},
		{"opacity", cfg.overlay.opacity},
		{"port", cfg.overlay.port},
		{"chat", {
			{"active", cfg.overlay.chat.active},
			{"position", cfg.overlay.chat.position},
			{"showHistory", cfg.overlay.chat.showHistory}
		}},
		{"gamepads", {
			{"active", cfg.overlay.gamepads.active},
			{"position", cfg.overlay.gamepads.position},
			{"showHotseatDuration", cfg.overlay.gamepads.showHotseatDuration}
		}},
		{"guests", {
			{"active", cfg.overlay.guests.active},
			{"position", cfg.overlay.guests.position},
			{"showLatency", cfg.overlay.guests.showLatency}
		}}
	};

	// Permissions
	j["Permissions"] = {
		{"guest", {
			{"useBB", cfg.permissions.guest.useBB},
			{"useSFX", cfg.permissions.guest.useSFX},
			{"changeControls", cfg.permissions.guest.changeControls}
		}},
		{"vip", {
			{"useBB", cfg.permissions.vip.useBB},
			{"useSFX", cfg.permissions.vip.useSFX},
			{"changeControls", cfg.permissions.vip.changeControls}
		}},
		{"moderator", {
			{"useBB", cfg.permissions.moderator.useBB},
			{"useSFX", cfg.permissions.moderator.useSFX},
			{"changeControls", cfg.permissions.moderator.changeControls}
		}}
	};

	// Arcade
	j["Arcade"] = {
		{"token", cfg.arcade.token},
		{"username", cfg.arcade.username},
		{"showLogin", cfg.arcade.showLogin}
	};

	// Save the file
	string configPath = GetConfigPath();
	if (configPath != "") {

		// Filepath
		string filePath = configPath + "\\config.json";

		string configString = j.dump(4);
		bool success = MTY_WriteTextFile(filePath.c_str(), "%s", configString.c_str());

	}

}

/// <summary>
/// Get the config file path.
/// </summary>
/// <returns></returns>
string Config::GetConfigPath() {
	
	// If running in portable mode
	string currentPath = GetCurrentPath();
	if (MTY_FileExists((currentPath + "\\portable.txt").c_str())) {
		return currentPath;
	} else {

		// Get the appdata path
		string appDataPath = GetAppDataPath();
		if (appDataPath != "") {
			return appDataPath;
		} else {
			return "";
		}

	}

	return "";
}

/// <summary>
/// Gets the current path of the application.
/// </summary>
/// <returns></returns>
string Config::GetCurrentPath() {
	char cwd[1024];
	if (_getcwd(cwd, sizeof(cwd)) != NULL) {
		return string(cwd);
	}
	else {
		return "";
	}
}

/// <summary>
/// Get the appdata path.
/// </summary>
/// <returns></returns>
string Config::GetAppDataPath() {
	string appDir = "\\SmashSodaTwo\\";

	TCHAR tAppdata[1024];
	if (SUCCEEDED(SHGetFolderPath(nullptr, CSIDL_APPDATA, NULL, 0, tAppdata))) {
		wstring wAppdata(tAppdata);
		string appdata(wAppdata.begin(), wAppdata.end());
		string dirPath = appdata + appDir;

		bool isDirOk = false;

		if (!MTY_FileExists(dirPath.c_str())) {
			if (MTY_Mkdir(dirPath.c_str())) {
				isDirOk = true;
			}
		} else {
			isDirOk = true;
		}

		if (isDirOk) {
			return dirPath;
		}
	}
}