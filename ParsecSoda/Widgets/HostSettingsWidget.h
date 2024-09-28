#pragma once

#define _WINSOCKAPI_

#include "../imgui/imgui.h"
#include "../ImGui/imform.h"
#include "../Hosting.h"
#include "../AudioTools.h"
#include "../Helpers/Debouncer.h"
#include "../globals/AppIcons.h"
#include "../globals/AppFonts.h"
#include "../globals/AppColors.h"
#include "../globals/AppStyle.h"
#include "ToggleIconButtonWidget.h"
#include "TitleTooltipWidget.h"
#include "TooltipWidget.h"
#include "ConfirmPopupWidget.h"
#include "IconButton.h"
#include "IntRangeWidget.h"
#include "BoolButtonWidget.h"
#include "AudioControlWidget.h"
#include <chrono>
#include <functional>
#include "../Core/Cache.h"
#include "../../Modules/Hotseat.h"

class HostSettingsWidget
{
public:
	HostSettingsWidget(Hosting& hosting, function<void(bool)> onHostRunningStatusCallback);
	bool render(HWND& hwnd);
	void renderGeneral(HWND& hwnd);
	void renderAudio();
	void renderHotseat();
	void renderKiosk();
	void updateSecretLink();

	const ImVec2 DEFAULT_BUTTON_SIZE = ImVec2(40, 40);
	const uint8_t LINK_COMPATIBLE_SECRET_SIZE = 9;

private:
	void savePreferences();
	bool validateSettings();
	bool isDirty();
	float lerp(float val1, float val2, float t = 0.11f);
	float easing(float t);

	// Dependency injection
	Hosting& _hosting;
	AudioIn& _audioIn;
	AudioOut& _audioOut;

	// Imported Widgets
	ToggleIconButtonWidget _publicRoomBtn, _playBtn;

	// Attributes
	char _gameID[GAME_ID_LEN];
	char _gameName[255];
	char _description[140];
	char _secret[HOST_SECRET_LEN];
	char _secretLink[128];
	bool _kioskMode;
	char _kioskApplication[256];
	char _kioskParam[256];
	char _shareLink[256];
	int32_t _maxGuests;
	bool _publicGame;
	bool _latencyLimiter;
	int32_t _latencyLimit;
	int _selectedArtwork = 0;
	int _selectedTheme = 0;

	// Hotseat
	bool _hotseat;
	bool _hotseatSlotMatch;
	int32_t _hotseatSeats;
	int32_t _hotseatTime;
	bool _hotseatAFK;
	int32_t _hotseatAFKTime;
	bool _hotseatPause;

	int _micVolume = 80;
	int _speakersVolume = 30;

	int _libraryID = -1;

	// Errors
	string _gameNameError = "";
	string _descriptionError = "";

	const unsigned int DEBOUNCE_TIME_MS = 1000;
	function<void(bool)> _onHostRunningStatusCallback;

	string _postThemes[6] = { "Default", "Black", "Blue", "Green", "Red"};
};

