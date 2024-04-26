#pragma once

#include "../imgui/imgui.h"
#include "../Hosting.h"
#include "../MetadataCache.h"
#include "../Helpers/Debouncer.h"
#include "../globals/AppIcons.h"
#include "../globals/AppFonts.h"
#include "../globals/AppColors.h"
#include "../globals/AppStyle.h"
#include "TitleTooltipWidget.h"
#include "TooltipWidget.h"
#include "IntRangeWidget.h"

class SettingsWidget
{
public:
	SettingsWidget(Hosting& hosting);
	bool render();
	void renderGeneral();
	void renderChatbot();
	void renderLog();
	void renderPermissions();

private:
	// Dependency injection
	Hosting& _hosting;
	uint32_t DEBOUNCER_TIME_MS = 2000;

	bool _microphoneEnabled = false;
	bool _disableGuideButton = false;
	bool _disableKeyboard = false;
	bool _latencyLimitEnabled = false;
	bool _leaderboardEnabled = false;
	bool _prependPingLimit = false;
	bool _autoIndex = false;
	bool _flashWindow = false;
	bool _sfxEnabled = false;
	bool _bonkEnabled = false;
	bool _hostBonkProof = false;
	unsigned int _latencyLimitThreshold = 0;
	unsigned int _theme = 0;
	char _discord[HOST_NAME_LEN] = "";
	char _chatbot[HOST_NAME_LEN] = "";
	char _welcomeMessage[256] = "";

	char _prependRegion[128] = "";

	int32_t _muteTime;
	bool _autoMute;
	int32_t _autoMuteTime;
	bool _saveChat;

	bool _guestBB = false;
	bool _vipBB = false;
	bool _modBB = false;
	
	bool _guestSFX = false;
	bool _vipSFX = false;
	bool _modSFX = false;

	string themes[5] = { "Midnight", "Parsec Soda", "Parsec Soda V", "Mini", "Arcade" };
};