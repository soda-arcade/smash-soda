#include "HostSettingsWidget.h"
#include "../ImGui/imform.h"

HostSettingsWidget::HostSettingsWidget(Hosting& hosting, function<void(bool)> onHostRunningStatusCallback)
    : _hosting(hosting), _audioIn(_hosting.audioIn), _audioOut(_hosting.audioOut), _onHostRunningStatusCallback(onHostRunningStatusCallback)
{
    ParsecHostConfig cfg = hosting.getHostConfig();
    try
    {
        strcpy_s(_roomName, cfg.name);
        strcpy_s(_gameID, cfg.gameID);
        strcpy_s(_gameName, MetadataCache::preferences.gameName.c_str());
		strcpy_s(_description, MetadataCache::preferences.description.c_str());
        strcpy_s(_secret, cfg.secret);
        strcpy_s(_kioskApplication, "");
        strcpy_s(_kioskParam, "");
    }
    catch (const std::exception&)
    {
        try
        {
            strcpy_s(_roomName, "");
            strcpy_s(_gameID, "");
			strcpy_s(_gameName, "");
			strcpy_s(_description, "");
            strcpy_s(_secret, "");
            strcpy_s(_kioskApplication, "");
            strcpy_s(_kioskParam, "");
        } catch (const std::exception&) {}
    }
	_publicGame = MetadataCache::preferences.publicRoom;
    _maxGuests = cfg.maxGuests;
    
    _micVolume = MetadataCache::preferences.micVolume;
    _audioIn.volume = (float)_micVolume / 100.0f;
    
    _speakersVolume = MetadataCache::preferences.speakersVolume;
    _audioOut.volume = (float)_speakersVolume / 100.0f;

    _audioIn.isEnabled = MetadataCache::preferences.micEnabled;
    _audioOut.isEnabled = MetadataCache::preferences.speakersEnabled;

    _latencyLimiter = MetadataCache::preferences.latencyLimitEnabled;
    _latencyLimit = MetadataCache::preferences.latencyLimitValue;

    _hotseat = MetadataCache::preferences.hotseat;
    _hotseatSeats = MetadataCache::preferences.hotseatSeats;
    _hotseatSlotMatch = MetadataCache::preferences.hotseatSlotMatch;
    _hotseatTime = MetadataCache::preferences.hotseatTime;
    _hotseatAFK = MetadataCache::preferences.hotseatAFK;
    _hotseatAFKTime = MetadataCache::preferences.hotseatAFKTime;
    _hotseatPause = MetadataCache::preferences.hotseatPause;

    _kioskMode = MetadataCache::preferences.kioskMode;

    if (strlen(_secret) == 0) {
        try { strcpy_s(_secret, "play-now"); }
        catch (const std::exception&) {}
    }
    updateSecretLink();

    if (strlen(_kioskApplication) == 0) {
        try { strcpy_s(_kioskApplication, MetadataCache::preferences.kioskApplication.c_str()); }
        catch (const std::exception&) {}
    }

    if (strlen(_kioskParam) == 0) {
        try { strcpy_s(_kioskParam, MetadataCache::preferences.kioskParameters.c_str()); }
        catch (const std::exception&) {}
    }
}

/// <summary>
/// Renders the host settings widget.
/// </summary>
bool HostSettingsWidget::render(HWND& hwnd) {
    
    AppStyle::pushTitle();
    ImGui::SetNextWindowSizeConstraints(ImVec2(150, 150), ImVec2(800, 900));
    ImGui::Begin("Hosting", (bool*)0);
    AppStyle::pushInput();

    ImVec2 size = ImGui::GetContentRegionAvail();

    ImGui::BeginChild("Hosting List", ImVec2(size.x, size.y));

    AppStyle::pushLabel();
    AppStyle::pop();
    ImGui::SetNextItemWidth(size.x - 10);

    renderGeneral(hwnd);

    ImGui::EndChild();

    AppStyle::pop();
    ImGui::End();
    AppStyle::pop();

    return true;

}

/// <summary>
/// Renders the general settings panel.
/// </summary>
void HostSettingsWidget::renderGeneral(HWND& hwnd) {

    static float indentSize = 0;
    static ImVec2 dummySize = ImVec2(0.0f, 10.0f);
    static ImVec2 cursor;
    static ImVec2 size;
    static ImVec2 pos;

    size = ImGui::GetContentRegionAvail();
    pos = ImGui::GetWindowPos();

    // Vector of themes
    AppStyle::pushLabel();
    ImGui::Text("GAME");
    AppStyle::pop();
    AppStyle::pushInput();
    ImGui::SetNextItemWidth(size.x);
    vector<GameData> games = _hosting.getGameList().getGames();
	string defaultValue = "Add games you own in the library widget.";

    int libraryID = MetadataCache::preferences.selectedGame;
    if (ImGui::BeginCombo("### Game picker combo", (
        games.size() == 0 || libraryID == 0 || libraryID > games.size() ? defaultValue.c_str() : games[libraryID - 1].name.c_str()
        ), ImGuiComboFlags_HeightLarge)) {
        // Game data list inumerator
		for (int i = 0; i < games.size(); ++i) {
			bool isSelected = (i == libraryID-1);
			if (ImGui::Selectable(games[i].name.c_str(), isSelected)) {
                MetadataCache::preferences.selectedGame = games[i].itemID;

                // Set room name
                string prepend = (MetadataCache::preferences.prependRegion != "" ? "[" + MetadataCache::preferences.prependRegion + "] " : "")
                    + (MetadataCache::preferences.prependPingLimit && _latencyLimiter
                        ? "[<" + to_string(_latencyLimit) + "ms] " : "")
                    + games[i].name;

                try { strcpy_s(_roomName, prepend.c_str()); }
                catch (const std::exception&) {}

                // Set game ID
                try { strcpy_s(_gameID, games[i].gameID.c_str()); }
                catch (const std::exception&) {}

                // Set game name
				MetadataCache::preferences.gameName = games[i].name;
                try { strcpy_s(_gameName, games[i].name.c_str()); }
                catch (const std::exception&) {}

                // Set hotseat
                MetadataCache::preferences.hotseat = games[i].hotseat;

                // Set hotseat seats
                MetadataCache::preferences.hotseatSeats = games[i].seats;

                // Kiosk mode
                MetadataCache::preferences.kioskMode = games[i].kiosk;
                
			}
            
            if (isSelected) {
				ImGui::SetItemDefaultFocus();
            }
            
		}
        ImGui::EndCombo();
    }
    AppStyle::pop();
    AppStyle::pushLabel();
	ImGui::TextWrapped("Automatically configure settings for a specific game in your library.");
    AppStyle::pop();
    
    ImGui::Dummy(dummySize);
    
    if (ImForm::InputText("ROOM NAME", _roomName, "The text displayed below thumbnails on the Arcade. No profanity please.")) {
        string prepend = (MetadataCache::preferences.prependRegion != "" ? "[" + MetadataCache::preferences.prependRegion + "]" : "")
			+ (MetadataCache::preferences.prependPingLimit && _latencyLimiter
                ? "[<" + to_string(_latencyLimit) + "ms]" : "")
            + _roomName;

        try { strcpy_s(_roomName, prepend.c_str()); }
        catch (const std::exception&) {}
    }

    if (ImForm::InputText("GAME NAME", _gameName, "Used for assigning correct box art on the Arcade.")) {
		// Process text here?
    }

	if (ImForm::InputText("DESCRIPTION", _description, 
        "A short description for your room displayed when hovering over your room on the Arcade. Max 140 characters. No profanity please.")) {
        // Limit to 140 characters
		if (strlen(_description) > 140) {
			_description[140] = '\0';
		}
    }

    AppStyle::pushLabel();
    ImGui::Text("SHARE LINK");
    AppStyle::pop();
    ImGui::SetNextItemWidth(size.x - 10);
    AppStyle::pushInput();
    ImGui::InputText("##Secret link", _secretLink, 128, ImGuiInputTextFlags_ReadOnly);
    AppStyle::pop();
    AppStyle::pushLabel();
    ImGui::TextWrapped("Users can join your room directly with this link.");
    AppStyle::pop();

    ImGui::Dummy(dummySize);
    ImGui::Dummy(dummySize);
    cursor = ImGui::GetCursorPos();

    AppStyle::pushLabel();

    ImGui::BeginChild("##Guest slot child", ImVec2(size.x / 3, 50.0f));
    ImGui::Text("GUEST SLOTS");
    if (IntRangeWidget::render("guest count", _maxGuests, 0, 64, 0.025f)) {
        
    }
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("##Public room child", ImVec2(size.x / 3, 50.0f));
    ImGui::Text("PUBLIC ROOM");
    ImGui::Indent(20);
    if (ToggleIconButtonWidget::render(AppIcons::yes, AppIcons::no, _publicGame, AppColors::positive, AppColors::negative, ImVec2(22, 22))) {
        _publicGame = !_publicGame;
		MetadataCache::preferences.publicRoom = _publicGame;
    }
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("##Latency child", ImVec2(size.x / 3, 50.0f));
    ImGui::Text("LATENCY LIMITER");
    if (ToggleIconButtonWidget::render(AppIcons::yes, AppIcons::no, _latencyLimiter, AppColors::positive, AppColors::negative, ImVec2(22, 22))) {
        _latencyLimiter = !_latencyLimiter;
    }
    ImGui::SameLine();

    if (IntRangeWidget::render("latency limit", _latencyLimit, 0, 64, 0.025f)) {
    }
    ImGui::EndChild();

    ImGui::Dummy(dummySize);
    cursor = ImGui::GetCursorPos();
	ImGui::Indent(0);

    ImGui::BeginChild("##Hotseat child", ImVec2(size.x / 3, 50.0f));
    ImGui::Text("HOTSEAT");
    ImGui::Indent(8);
    if (ToggleIconButtonWidget::render(AppIcons::yes, AppIcons::no, MetadataCache::preferences.hotseat, AppColors::positive, AppColors::negative, ImVec2(22, 22))) {
        MetadataCache::preferences.hotseat = !MetadataCache::preferences.hotseat;
        if (_hosting.isRunning()) {
			if (MetadataCache::preferences.hotseat) {
                _hosting.getHotseat().start();
			}
			else {
				_hosting.getHotseat().stop();
			}
        }
    }
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("##Kiosk child", ImVec2(size.x / 3, 50.0f));
    ImGui::Text("KIOSK MODE");
    ImGui::Indent(15);
    if (ToggleIconButtonWidget::render(AppIcons::yes, AppIcons::no, MetadataCache::preferences.kioskMode, AppColors::positive, AppColors::negative, ImVec2(22, 22))) {
        MetadataCache::preferences.kioskMode = !MetadataCache::preferences.kioskMode;
        if (_hosting.isRunning()) {
            if (MetadataCache::preferences.kioskMode) {
                _hosting.startKioskMode();
            }
            else {
                _hosting.getProcessMan().stop();
            }
        }
    }
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("##Overlay child", ImVec2(size.x / 3, 50.0f));
    ImGui::Indent(20);
    ImGui::Text("OVERLAY");
    ImGui::Indent(8);
    if (ToggleIconButtonWidget::render(AppIcons::yes, AppIcons::no, MetadataCache::preferences.overlayShow, AppColors::positive, AppColors::negative, ImVec2(22, 22))) {
        MetadataCache::preferences.overlayShow = !MetadataCache::preferences.overlayShow;
        if (_hosting.isRunning()) {
            if (_hosting.getOverlay().isActive) {
                _hosting.getOverlay().stop();
			} else {
				_hosting.getOverlay().start();
			}
        }
    }
    ImGui::EndChild();

    AppStyle::pop();

    ImGui::Dummy(dummySize);

    static bool showPopup = false;
    static string popupTitle = "";
    popupTitle = (_hosting.isRunning() ? "Stop hosting?" : "Start hosting?");

    indentSize = 0.5f * size.x - 60.0f;
    ImGui::Indent(indentSize);
    if (ToggleIconButtonWidget::render(
        AppIcons::stop, AppIcons::play, _hosting.isRunning(),
        AppColors::negative, AppColors::positive, ImVec2(100, 100)
    ))
    {
        showPopup = true;
        ImGui::OpenPopup(popupTitle.c_str());

        savePreferences();
    }
    ImGui::Unindent(indentSize);

    if (_hosting.isRunning())   TitleTooltipWidget::render("Stop hosting", "Close current room.");
    else                        TitleTooltipWidget::render("Start hosting", "Open a new room using these settings.");

    // ================================================================================

    if (ConfirmPopupWidget::render(popupTitle.c_str(), showPopup))
    {
        // Was clicked and is already running (must stop)
        if (_hosting.isRunning())
        {
            _hosting.stopHosting();
            if (_onHostRunningStatusCallback != nullptr) _onHostRunningStatusCallback(false);
        }

        // Was clicked and is not running (must start)
        else
        {
            MetadataCache::preferences.latencyLimitEnabled = _latencyLimiter;
            MetadataCache::preferences.latencyLimitValue = _latencyLimit;

            MetadataCache::preferences.gameName = _gameName;
            MetadataCache::preferences.description = _description;

            _hosting.setHostConfig(_roomName, _gameID, _maxGuests, false, _secret);
            _hosting.applyHostConfig();
            _hosting.startHosting();
            if (_onHostRunningStatusCallback != nullptr) _onHostRunningStatusCallback(true);
        }
    }

    if (_hosting.isRunning() && isDirty()) {

        ImGui::Dummy(dummySize);
        indentSize = 0.5f * size.x - 65.0f;
        ImGui::Indent(indentSize);

        AppColors::pushInput();
        AppFonts::pushTitle();
        ImGui::PushStyleColor(ImGuiCol_Button, AppColors::primary);
        if (ImGui::Button("Update Settings")) {
			MetadataCache::preferences.gameName = _gameName;
            MetadataCache::preferences.description = _description;
            _hosting.setHostConfig(_roomName, _gameID, _maxGuests, _publicGame, _secret);
            _hosting.applyHostConfig();
            _hosting.parsecArcadeUpdate();
            savePreferences();
        }
        TitleTooltipWidget::render("Update Room Settings", "The room will be instantly updated with your new settings.");
        ImGui::PopStyleColor();
        AppStyle::pop();

    }

    ImGui::Dummy(ImVec2(0.0f, 20.0f));

}

/// <summary>
/// Renders the audio settings panel.
/// </summary>
void HostSettingsWidget::renderAudio() {

    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    if (!_hosting.isRunning() && _hosting.isReady())
    {
        if (!_hosting._disableMicrophone) _audioIn.captureAudio();
        _audioOut.captureAudio();
    }

    static int previousMicVolume, previousSpeakersVolume;
    static bool isVolumeChanged = false;
    static float targetPreview;

    AppStyle::pushLabel();
    ImGui::Text("SPEAKERS");

    static float speakersPreview;
    _speakersVolume = (int)(100.0f * _audioOut.volume);
    targetPreview = AudioTools::decibelToFloat(_audioOut.popPreviewDecibel());
    speakersPreview = lerp(speakersPreview, targetPreview, easing(targetPreview - speakersPreview));
    if (AudioControlWidget::render("Speakers", &_speakersVolume, _audioOut.isEnabled, speakersPreview, AppIcons::speakersOn, AppIcons::speakersOff))
    {
        _audioOut.isEnabled = !_audioOut.isEnabled;
        savePreferences();
    }
    _audioOut.volume = (float)_speakersVolume / 100.0f;

    static Debouncer debouncer(DEBOUNCE_TIME_MS, [&]() { savePreferences(); });
    if (_micVolume != previousMicVolume || _speakersVolume != previousSpeakersVolume)
    {
        debouncer.start();
    }

    AppStyle::pushLabel();
    ImGui::Text("MICROPHONE");

    if (!_hosting._disableMicrophone)
    {

        previousMicVolume = _micVolume;
        previousSpeakersVolume = _speakersVolume;

        static float micPreview;
        _micVolume = (int)(100.0f * _audioIn.volume);
        targetPreview = AudioTools::decibelToFloat(_audioIn.popPreviewDecibel());
        micPreview = lerp(micPreview, targetPreview, easing(targetPreview - micPreview));
        if (AudioControlWidget::render("Microphone", &_micVolume, _audioIn.isEnabled, micPreview, AppIcons::micOn, AppIcons::micOff))
        {
            _audioIn.isEnabled = !_audioIn.isEnabled;
            savePreferences();
        }
        _audioIn.volume = (float)_micVolume / 100.0f;

    }

    ImGui::Dummy(ImVec2(0.0f, 20.0f));

}

/// <summary>
/// Renders the hotseat settings panel.
/// </summary>
void HostSettingsWidget::renderHotseat() {

    static ImVec2 size;
    static ImVec2 pos;

    size = ImGui::GetContentRegionAvail();
    pos = ImGui::GetWindowPos();

    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    _hotseat = MetadataCache::preferences.hotseat;
    if (ImForm::InputToggle("HOTSEAT", _hotseat,
        "When hotseat mode is enabled, the active gamepad(s) will be swapped between your guests.")) {
        MetadataCache::preferences.hotseat = _hotseat;
        MetadataCache::savePreferences();
    }

    if (ImForm::InputNumber("SEATS", _hotseatSeats, 1, 8,
        "The number of guests that can be put in to hotseat(s).")) {
        MetadataCache::preferences.hotseatSeats = _hotseatSeats;
        MetadataCache::savePreferences();
    }

    _hotseatSlotMatch = MetadataCache::preferences.hotseatSlotMatch;
    if (ImForm::InputCheckbox("Xinput slots must match?", _hotseatSlotMatch,
        "Most games require player 1 to use xinput slot 1, player 2 in slot 2 etc. The host should have their controller disconnected before enabling this.")) {
        MetadataCache::preferences.hotseatSlotMatch = _hotseatSlotMatch;
        MetadataCache::savePreferences();
    }

    if (ImForm::InputNumber("HOTSEAT TIME", _hotseatTime, 5, 60,
        "The amount of time the hotseat guest(s) has to play.")) {
        MetadataCache::preferences.hotseatTime = _hotseatTime;
        MetadataCache::savePreferences();
    }

    if (ImForm::InputCheckbox("AFK Strip", _hotseatAFK,
        "If the current hotseat guest goes AFK for a set period of time, they will automatically be set to spectate.")) {
        MetadataCache::preferences.hotseatAFK = _hotseatAFK;
        MetadataCache::savePreferences();
    }

    if (ImForm::InputNumber("AFK TIME", _hotseatAFKTime, 1, 60,
        "If AFK STRIP is enabled, then if the hotseat guest is away for this amount of time they'll be moved to spectate.")) {
        MetadataCache::preferences.hotseatAFKTime = _hotseatAFKTime;
        MetadataCache::savePreferences();
    }

    if (ImForm::InputCheckbox("Pause on Swap", _hotseatPause,
        "Have the start button be pressed automatically when the hotseat guest is swapped.")) {
        MetadataCache::preferences.hotseatPause = _hotseatPause;
        MetadataCache::savePreferences();
    }

    ImGui::Dummy(ImVec2(0.0f, 20.0f));

}

/// <summary>
/// Renders the kiosk mode settings panel.
/// </summary>
void HostSettingsWidget::renderKiosk() {

    static ImVec2 size;
    static ImVec2 pos;

    size = ImGui::GetContentRegionAvail();
    pos = ImGui::GetWindowPos();

    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    AppStyle::pushLabel();
    ImGui::Text("KIOSK MODE");
    if (ToggleIconButtonWidget::render(AppIcons::yes, AppIcons::no, _kioskMode, AppColors::positive, AppColors::negative, ImVec2(22, 22))) {
        MetadataCache::preferences.kioskMode = !MetadataCache::preferences.kioskMode;
        _kioskMode = MetadataCache::preferences.kioskMode;
    }
    if (MetadataCache::preferences.kioskMode)    TitleTooltipWidget::render("Kiosk Mode Off", "Don't launch in kiosk mode.");
    else                    TitleTooltipWidget::render("Kiosk Mode On", "Automatically restart a program if it is closed.");
    AppStyle::pushLabel();
    ImGui::TextWrapped("When kiosk mode is enabled, if the game/app you are hosting is stopped, it will automatically be restarted again.");
    AppStyle::pop();

    ImGui::Dummy(ImVec2(0, 20.0f));

    AppStyle::pushLabel();
    ImGui::Text("APPLICATION FILE PATH");

    ImGui::SetNextItemWidth(size.x - 10);
    AppStyle::pushInput();
    if (ImGui::InputText("##Kioskapplication", _kioskApplication, 256)) {
        MetadataCache::preferences.kioskApplication = _kioskApplication;
    }
    TitleTooltipWidget::render("Kiosk Application File Path", "Set the file path of the program you wish to restart.");

    AppStyle::pushLabel();
    ImGui::TextWrapped("The file path of the program you wish to host (this will be started automatically when hosting starts).");
    AppStyle::pop();

    ImGui::Dummy(ImVec2(0, 10.0f));

    AppStyle::pushLabel();
    ImGui::Text("LAUNCH PARAMETERS");

    ImGui::SetNextItemWidth(size.x - 10);
    AppStyle::pushInput();
    if (ImGui::InputText("##Kioskparameters", _kioskParam, 256)) {
        MetadataCache::preferences.kioskParameters = _kioskParam;
    }
    TitleTooltipWidget::render("Kiosk Parameters", "Pass any additional parameters to the kiosk application.");

    AppStyle::pushLabel();
    ImGui::TextWrapped("If the program requires launch parameters you can set them here.");
    AppStyle::pop();

    ImGui::Dummy(ImVec2(0.0f, 20.0f));

}

/// <summary>
/// Saves the new preferences.
/// </summary>
void HostSettingsWidget::savePreferences() {
    MetadataCache::preferences.roomName = _roomName;
    MetadataCache::preferences.gameID = strlen(_gameID) <= 25 ? "1wdoHfhhZH5lPuZCwGBete0HIAj" : _gameID;
    MetadataCache::preferences.guestCount = _maxGuests;
    MetadataCache::preferences.publicRoom = _publicGame;
    MetadataCache::preferences.secret = _secret;
    MetadataCache::preferences.micVolume = _micVolume;
    MetadataCache::preferences.micEnabled = _audioIn.isEnabled;
    MetadataCache::preferences.speakersVolume = _speakersVolume;
    MetadataCache::preferences.speakersEnabled = _audioOut.isEnabled;
    MetadataCache::preferences.latencyLimitEnabled = _latencyLimiter;
    MetadataCache::preferences.latencyLimitValue = _latencyLimit;
    MetadataCache::preferences.kioskApplication = _kioskApplication;
    MetadataCache::preferences.kioskParameters = _kioskParam;
    MetadataCache::savePreferences();
}

/// <summary>
/// Updates the secret link used when hosting room privately.
/// </summary>
void HostSettingsWidget::updateSecretLink() {

    try
    {
        strcpy_s(
            _secretLink,
            128,
            (string("https://parsec.gg/g/") + _hosting.getSession().hostPeerId + "/" + _secret + "/").c_str()
        );
    }
    catch (const std::exception&) {}

}

/// <summary>
/// If any settings have been changed once hosting is started,
/// then these settings are "dirty".
/// </summary>
bool HostSettingsWidget::isDirty() {
    ParsecHostConfig cfg = _hosting.getHostConfig();

    if (
        _publicGame != cfg.publicGame ||
        _maxGuests != cfg.maxGuests ||
        _latencyLimiter != MetadataCache::preferences.latencyLimitEnabled ||
        _latencyLimit != MetadataCache::preferences.latencyLimitValue ||
        (strcmp(_roomName, cfg.name) != 0) ||
        (strcmp(_gameID, cfg.gameID) != 0) ||
        (strcmp(_secret, cfg.secret) != 0)
    ) return true;

    return false;
}

/// <summary>
/// Helper lerp function.
/// </summary>
float HostSettingsWidget::lerp(float val1, float val2, float t) {
    return (1-t) * val1 + t * val2;
}

/// <summary>
/// Helper easing function.
/// </summary>
float HostSettingsWidget::easing(float t) {
    return 0.3f * max(abs(t), 0.4f);
}
