#include "HostSettingsWidget.h"
#include "../ImGui/imform.h"

HostSettingsWidget::HostSettingsWidget(Hosting& hosting, function<void(bool)> onHostRunningStatusCallback)
    : _hosting(hosting), _audioIn(_hosting.audioIn), _audioOut(_hosting.audioOut), _onHostRunningStatusCallback(onHostRunningStatusCallback)
{
    ParsecHostConfig cfg = hosting.getHostConfig();
    try
    {
        strcpy_s(_gameID, cfg.gameID);
        strcpy_s(_gameName, (Config::cfg.room.game.size() < 255 ? Config::cfg.room.game.c_str() : ""));
		strcpy_s(_description, (Config::cfg.room.details.size() < 500 ? Config::cfg.room.details.c_str() : ""));
        strcpy_s(_secret, cfg.secret);
        strcpy_s(_kioskApplication, "");
        strcpy_s(_kioskParam, "");
        strcpy_s(_streamUrl, (Config::cfg.room.streamUrl.size() < 255 ? Config::cfg.room.streamUrl.c_str() : ""));
    }
    catch (const std::exception&)
    {
        try
        {
            strcpy_s(_gameID, "");
			strcpy_s(_gameName, "");
			strcpy_s(_description, "");
            strcpy_s(_secret, "");
            strcpy_s(_kioskApplication, "");
            strcpy_s(_kioskParam, "");
            strcpy_s(_streamUrl, "");
        } catch (const std::exception&) {}
    }
    _maxGuests = Config::cfg.room.guestLimit;
    _repThreshold = Config::cfg.room.repThreshold;
    
    _micVolume = Config::cfg.audio.micVolume;
    _audioIn.volume = (float)_micVolume / 100.0f;
    
    _speakersVolume = Config::cfg.audio.speakersVolume;
    _audioOut.volume = (float)_speakersVolume / 100.0f;

    _audioIn.isEnabled = Config::cfg.audio.micEnabled;
    _audioOut.isEnabled = Config::cfg.audio.speakersEnabled;

    _latencyLimiter = Config::cfg.room.latencyLimit;
    _latencyLimit = Config::cfg.room.latencyLimitThreshold;

    _hotseat = Config::cfg.hotseat.enabled;

    _kioskMode = Config::cfg.kioskMode.enabled;

    if (strlen(_secret) == 0) {
        try { strcpy_s(_secret, "play-now"); }
        catch (const std::exception&) {}
    }
    updateSecretLink();

}

/// <summary>
/// Validate settings before sending to Soda Arcade.
/// </summary>
/// <returns></returns>
bool HostSettingsWidget::validateSettings() {

	if (strlen(_gameName) < 3 || strlen(_gameName) > 255) {
		_validateError = "Game name must be between 3 and 255 characters.";
        _showErrorPopup = true;
        ImGui::OpenPopup("ERROR");
		return false;
	}

	// Details can't be longer than 500 characters
	if (strlen(_description) > 500) {
        _validateError = "Description can't be longer than 500 characters.";
        _showErrorPopup = true;
        ImGui::OpenPopup("ERROR");
		return false;
	}

	return true;
    
}

bool HostSettingsWidget::render(bool& showWindow, HWND& hwnd) {

    AppStyle::pushTitle();
    ImGui::SetNextWindowPos(ImVec2(464, 5), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSizeConstraints(ImVec2(400, 500), ImVec2(800, 1000));
    ImGui::Begin("Hosting", &showWindow);
    if (!showWindow) Config::cfg.widgets.host = showWindow;
    AppStyle::pushInput();

    ImVec2 size = ImGui::GetContentRegionAvail();

    ImGui::BeginChild("content", ImVec2(size.x, size.y - 60.0f));
    if (ImGui::BeginTabBar("Hosting Tabs", ImGuiTabBarFlags_None)) {
        AppFonts::pushInput();
        AppColors::pushTitle();
        if (ImGui::BeginTabItem("General")) {
            ImGui::BeginChild("innerscroll");
            renderGeneral(hwnd);
            ImGui::EndChild();
            ImGui::EndTabItem();
        }
        if (!Config::cfg.room.privateRoom) {
            if (ImGui::BeginTabItem("Advanced")) {
                ImGui::BeginChild("innerscroll");
                renderAdvanced();
                ImGui::EndChild();
                ImGui::EndTabItem();
            }
        }
        AppColors::pop();
        AppFonts::pop();
        ImGui::EndTabBar();
    }
    ImGui::EndChild();

    static float indentSize = 0;
    static ImVec2 dummySize = ImVec2(0.0f, 5.0f);
    static ImVec2 cursor;
    static ImVec2 pos;

    size = ImGui::GetContentRegionAvail();
    pos = ImGui::GetWindowPos();

    ImGui::Dummy(dummySize);

    ImGui::Separator();

    ImGui::Dummy(ImVec2(0, 5.0f));

    static bool showPopup = false;
    static std::string popupTitle = "";
    popupTitle = (_hosting.isRunning() ? "Stop hosting?" : "Start hosting?");

    if (!_hosting.isRunning()) {
        ImGui::BeginGroup();
        AppFonts::pushLarge();
        AppColors::pushButtonSolid();
        if (ImGui::Button("START HOSTING")) {
            showPopup = true;
            ImGui::OpenPopup(popupTitle.c_str());

            savePreferences();
        }
        AppColors::popButton();
        AppFonts::pop();
        ImGui::EndGroup();
    }
    else {

        ImGui::BeginGroup();
        AppColors::pushButtonDanger();
        AppFonts::pushLarge();
        if (ImGui::Button("STOP HOSTING")) {
            showPopup = true;
            ImGui::OpenPopup(popupTitle.c_str());
        }
        AppFonts::pop();
        AppColors::popButton();
        ImGui::EndGroup();

        ImGui::SameLine();

        if (isDirty()) {
            cursor = ImGui::GetCursorPos();
            ImGui::SetCursorPos(ImVec2(size.x - 60, cursor.y));
            ImGui::BeginGroup();
            AppColors::pushButtonSecondary();
            AppFonts::pushLarge();
            if (ImGui::Button("UPDATE")) {

                // Apply settings for Parsec
                _hosting.setHostConfig("", _gameID, _maxGuests, false, _secret);
                _hosting.applyHostConfig();

                savePreferences();

                // Update on Soda Arcade
                if (!Config::cfg.room.privateRoom) {
                    Arcade::instance.createPost();
                }

                _updated = false;
            }
            AppFonts::pop();
            AppColors::popButton();
            ImGui::EndGroup();
        }

    }

    //// ================================================================================

    // Center to the screen
    ImGui::SetCursorPos(ImVec2(0.5f * size.x - 130.0f, (ImGui::GetCursorPos().y / 2) - 80));

    if (ConfirmPopupWidget::render(popupTitle.c_str(), showPopup)) {
        // Was clicked and is already running (must stop)
        if (_hosting.isRunning()) {
            _hosting.stopHosting();
            if (_onHostRunningStatusCallback != nullptr) _onHostRunningStatusCallback(false);
            _updated = false;
        }

        // Was clicked and is not running (must start)
        else if (Config::cfg.room.privateRoom || validateSettings()) {
            savePreferences();

            _hosting.setHostConfig("", _gameID, _maxGuests, false, _secret);
            _hosting.applyHostConfig();
            _hosting.startHosting();
            if (_onHostRunningStatusCallback != nullptr) _onHostRunningStatusCallback(true);

            // Start kiosk mode
            if (Config::cfg.kioskMode.enabled) {
                ProcessMan::instance.stop();
                std::vector<GameData> games = Cache::cache.gameList.getGames();
                ProcessMan::instance.start(games[_libraryID].path, games[_libraryID].parameters);
            }
        }
    }

    // Errors
    ImGui::SetCursorPos(ImVec2(0.5f * size.x - 130.0f, (ImGui::GetCursorPos().y / 2) - 80));
    ConfirmPopupWidget::render("ERROR", _showErrorPopup, _validateError);

    ImGui::End();

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

    ImGui::Dummy(dummySize);

    // Vector of themes
    AppStyle::pushLabel();
    ImGui::Text("LIBRARY");
    AppStyle::pop();
    AppStyle::pushInput();
    ImGui::SetNextItemWidth(size.x);
    vector<GameData> games = Cache::cache.gameList.getGames();
	string emptyValue = "Add games you own in the library widget.";

    bool isValid = (games.size() > 0 && _libraryID > -1 && _libraryID <= games.size());

    // Game picker
    if (ImGui::BeginCombo("### Game picker combo", (
        isValid ? games[_libraryID].name.c_str() : emptyValue.c_str()
        ), ImGuiComboFlags_HeightLarge)) {
        // Game data list inumerator
		for (int i = 0; i < games.size(); ++i) {
			bool isSelected = (i == _libraryID);

            // Set the game
			if (ImGui::Selectable(games[i].name.c_str(), isSelected)) {
                _libraryID = i;
                try { strcpy_s(_gameName, games[i].name.c_str()); }
                catch (const std::exception&) {}    
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

    if (ImForm::InputText("GAME NAME", _gameName, "Required. The game you are hosting. Be sure to write the exact name of the game to find correct cover art.")) {
	
        // Can't be longer than 255 characters
        if (strlen(_gameName) > 255) {
            string name = _gameName;
            name = name.substr(0, 255);
            strcpy_s(_gameName, name.c_str());
        }

    }

    char link[256];
    if (Config::cfg.room.privateRoom) {
        strcpy_s(link, _secretLink);
	}
    else {
		string arcadeLink = "https://soda-arcade.com/invite/" + Arcade::instance.credentials.username + "/room";
        strcpy_s(link, arcadeLink.c_str());
	}

    AppStyle::pushLabel();
    ImGui::Text("SHARE LINK");
    AppStyle::pop();
    ImGui::SetNextItemWidth(size.x - 10);
    AppStyle::pushInput();
    ImGui::InputText("##Secret link", link, 128, ImGuiInputTextFlags_ReadOnly);
    AppStyle::pop();
    AppStyle::pushLabel();
    ImGui::TextWrapped("Users can join your room directly with this link.");
    AppStyle::pop();

    ImGui::Dummy(dummySize);
    ImGui::Dummy(dummySize);
    cursor = ImGui::GetCursorPos();

    AppStyle::pushLabel();

    ImGui::BeginChild("##Button group", ImVec2(size.x, 120.0f));
        ImGui::BeginChild("##Guest slot child", ImVec2(size.x / 3, 50.0f));
        ImGui::Text("GUEST SLOTS");
        if (IntRangeWidget::render("guest count", _maxGuests, 0, 20, 0.025f)) {
		    Config::cfg.room.guestLimit = _maxGuests;
            if (_hosting.isRunning()) {
                _updated = true;
            }
        }
        ImGui::EndChild();

        ImGui::SameLine();

        ImGui::BeginChild("##Public room child", ImVec2(size.x / 3, 50.0f));
        ImGui::Text("PUBLIC ROOM");
        ImGui::Indent(20);
        if (ToggleIconButtonWidget::render(AppIcons::yes, AppIcons::no, !Config::cfg.room.privateRoom, AppColors::positive, AppColors::negative, ImVec2(22, 22))) {
		    // If public game is false, check we have authenticated with Soda Arcade
		    if (Config::cfg.room.privateRoom) {
			    if (Arcade::instance.credentials.token.empty()) {
				    Config::cfg.arcade.showLogin = true;
			    }
			    else {
				    Config::cfg.room.privateRoom = false;
			    }
		    }
		    else {
			    Config::cfg.room.privateRoom = true;
		    }

            if (_hosting.isRunning()) {
                _updated = true;
            }
        }
        ImGui::EndChild();

        ImGui::SameLine();

        ImGui::BeginChild("##Latency child", ImVec2(size.x / 3, 50.0f));
        ImGui::Text("LATENCY LIMITER");
        if (ToggleIconButtonWidget::render(AppIcons::yes, AppIcons::no, _latencyLimiter, AppColors::positive, AppColors::negative, ImVec2(22, 22))) {
            _latencyLimiter = !_latencyLimiter;
            if (_hosting.isRunning()) {
                _updated = true;
            }
        }
        ImGui::SameLine();

        if (IntRangeWidget::render("latency limit", _latencyLimit, 0, 64, 0.025f)) {
            Config::cfg.room.latencyLimitThreshold = _latencyLimit;
            if (_hosting.isRunning()) {
				_updated = true;
			}
        }
        ImGui::EndChild();

        ImGui::Dummy(dummySize);
        cursor = ImGui::GetCursorPos();
	    ImGui::Indent(0);

        ImGui::BeginChild("##Hotseat child", ImVec2(size.x / 3, 50.0f));
        ImGui::Text("HOTSEAT");
        ImGui::Indent(8);
        if (ToggleIconButtonWidget::render(AppIcons::yes, AppIcons::no, Config::cfg.hotseat.enabled, AppColors::positive, AppColors::negative, ImVec2(22, 22))) {
            Config::cfg.hotseat.enabled = !Config::cfg.hotseat.enabled;
        }
        ImGui::EndChild();

        ImGui::SameLine();

        ImGui::BeginChild("##Kiosk child", ImVec2(size.x / 3, 50.0f));
        ImGui::Text("KIOSK MODE");
        ImGui::Indent(15);
        if (ToggleIconButtonWidget::render(AppIcons::yes, AppIcons::no, Config::cfg.kioskMode.enabled, AppColors::positive, AppColors::negative, ImVec2(22, 22))) {
            // Can only start kiosk mode when a game is selected
            if (_libraryID == -1) {
                Config::cfg.kioskMode.enabled = false;
            }
            else {
                Config::cfg.kioskMode.enabled = !Config::cfg.kioskMode.enabled;
                if (_hosting.isRunning()) {
                    if (Config::cfg.kioskMode.enabled && _libraryID != -1) {
                        ProcessMan::instance.start(games[_libraryID].path, games[_libraryID].parameters);
                    }
                    else {
                        ProcessMan::instance.stop();
                    }
                }
            }
        }
        TitleTooltipWidget::render("Kiosk Mode", "Kiosk mode can only be started when you have selected a game from your library.");

        ImGui::EndChild();

        ImGui::SameLine();

        ImGui::BeginChild("##Overlay child", ImVec2(size.x / 3, 50.0f));
        ImGui::Indent(20);
        ImGui::Text("OVERLAY");
        ImGui::Indent(8);
        if (ToggleIconButtonWidget::render(AppIcons::yes, AppIcons::no, Config::cfg.overlay.enabled, AppColors::positive, AppColors::negative, ImVec2(22, 22))) {
            if (_hosting.isRunning()) {
                if (Config::cfg.overlay.enabled) {
                    WebSocket::instance.closeOverlay();
                }
                else {
                    WebSocket::instance.launchOverlay();
                }
            }
            Config::cfg.overlay.enabled = !Config::cfg.overlay.enabled;
        }
        ImGui::EndChild();
    ImGui::EndChild();

    AppStyle::pop();

}

/// <summary>
/// Renders the audio settings panel.
/// </summary>
void HostSettingsWidget::renderAdvanced() {

    static float indentSize = 0;
    static ImVec2 dummySize = ImVec2(0.0f, 10.0f);
    static ImVec2 cursor;
    static ImVec2 size;
    static ImVec2 pos;

    size = ImGui::GetContentRegionAvail();
    pos = ImGui::GetWindowPos();

    ImGui::Dummy(dummySize);

    if (Arcade::instance.artwork.size() > 0) {
        AppStyle::pushLabel();
        ImGui::Text("CUSTOM ARTWORK");
        AppStyle::pop();
        AppStyle::pushInput();
        ImGui::SetNextItemWidth(size.x);

        std::string defaultValue = "None";
        if (_selectedArtwork != -1) {
            defaultValue = Arcade::instance.artwork[_selectedArtwork].title;
        }

        if (ImGui::BeginCombo("### Artwork picker combo", defaultValue.c_str(), ImGuiComboFlags_HeightLarge)) {

            if (ImGui::Selectable("None", (_selectedArtwork == -1))) {
                _selectedArtwork = -1;
                Arcade::instance.artworkID = -1;
            }
            if (_selectedArtwork == -1) {
                ImGui::SetItemDefaultFocus();
            }

            for (size_t i = 0; i < Arcade::instance.artwork.size(); ++i) {
                bool isSelected = (i == _selectedArtwork);
                if (ImGui::Selectable(Arcade::instance.artwork[i].title.c_str(), isSelected)) {
                    _selectedArtwork = i;
                    Arcade::instance.artworkID = Arcade::instance.artwork[i].id;
                    if (_hosting.isRunning()) {
                        _updated = true;
                    }
                }
                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
        AppStyle::pop();
        AppStyle::pushLabel();
        ImGui::TextWrapped("If you have any custom artwork uploaded to Soda Arcade, you can select it here.");
        AppStyle::pop();
        ImGui::Dummy(ImVec2(0, 10.0f));
    }

    AppStyle::pushLabel();
    ImGui::Text("POST THEME");
    AppStyle::pop();
    AppStyle::pushInput();
    ImGui::SetNextItemWidth(size.x);
    if (ImGui::BeginCombo("### Post theme picker combo", _postThemes[_selectedTheme].c_str(), ImGuiComboFlags_HeightLarge)) {

        for (size_t i = 0; i < 6; ++i) {
            bool isSelected = (i == _selectedTheme);
            if (ImGui::Selectable(_postThemes[i].c_str(), isSelected)) {
                _selectedTheme = i;
                if (_hosting.isRunning()) {
                    _updated = true;
                }
            }
            if (isSelected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
    AppStyle::pop();
    AppStyle::pushLabel();
    ImGui::TextWrapped("This lets you change the colour theme of your room poster on Soda Arcade.");
    AppStyle::pop();
    ImGui::Dummy(ImVec2(0, 10.0f));

    if (ImForm::InputTextArea("DETAILS", _description,
        "Any additional details about the game or your room. This will be displayed when somebody clicks your post.")) {
        if (strlen(_description) > 500) {
            string desc = _description;
            desc = desc.substr(0, 500);
            strcpy_s(_description, desc.c_str());
        }

        Config::cfg.room.details = _description;

        if (_hosting.isRunning()) {
            _updated = true;
        }
    }

    if (ImForm::InputText("STREAM CHANNEL URL", _streamUrl, "Here you can enter your Twitch or Kick channel URL if you want to display your stream on your room post.")) {

        // Can't be longer than 255 characters
        if (strlen(_gameName) > 255) {
            string streamUrl = _streamUrl;
            streamUrl = streamUrl.substr(0, 255);
            strcpy_s(_streamUrl, streamUrl.c_str());
        }

        Config::cfg.room.streamUrl = _streamUrl;

        if (_hosting.isRunning()) {
            _updated = true;
        }

    }

    if (ImForm::InputNumber("REP THRESHOLD", _repThreshold, 10, 100,
        "Users with reputation below this level won't see your room on the arcade.")) {
        Config::cfg.room.repThreshold = _repThreshold;

        if (_hosting.isRunning()) {
            _updated = true;
        }
    }

}

/// <summary>
/// Renders the audio settings panel.
/// </summary>
void HostSettingsWidget::renderAudio() {

    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    if (!_hosting.isRunning() && _hosting.isReady())
    {
        _audioIn.captureAudio();
        _audioOut.captureAudio();
    }

    static int previousMicVolume, previousSpeakersVolume;
    static bool isVolumeChanged = false;
    previousMicVolume = _micVolume;
    previousSpeakersVolume = _speakersVolume;

    // ====================================================
    // Microphone
    // ====================================================
    static float micPreview, targetPreview;
    _micVolume = (int)(100.0f * _audioIn.volume);
    targetPreview = AudioTools::decibelToFloat(_audioIn.popPreviewDecibel());
    micPreview = lerp(micPreview, targetPreview, easing(targetPreview - micPreview));

    const static auto volumeReleaseCallback = [&]() {
        savePreferences();
    };

    if (AudioControlWidget::render("Microphone", &_micVolume, _audioIn.isEnabled, micPreview, AppIcons::micOn, AppIcons::micOff, volumeReleaseCallback))
    {
        _audioIn.isEnabled = !_audioIn.isEnabled;
        savePreferences();
    }
    _audioIn.volume = (float)_micVolume / 100.0f;

    // ====================================================
    // Speakers
    // ====================================================
    static float speakersPreview;
    _speakersVolume = (int)(100.0f *_audioOut.volume);
    targetPreview = AudioTools::decibelToFloat(_audioOut.popPreviewDecibel());
    speakersPreview = lerp(speakersPreview, targetPreview, easing(targetPreview - speakersPreview));
    if (AudioControlWidget::render("Speakers", &_speakersVolume, _audioOut.isEnabled, speakersPreview, AppIcons::speakersOn, AppIcons::speakersOff, volumeReleaseCallback))
    {
        _audioOut.isEnabled = !_audioOut.isEnabled;
        savePreferences();
    }
    _audioOut.volume = (float)_speakersVolume / 100.0f;

    AppStyle::pop();
    ImGui::End();
    AppStyle::pop();

}

/// <summary>
/// Saves the new preferences.
/// </summary>
void HostSettingsWidget::savePreferences() {
	Config::cfg.room.game = _gameName;
	Config::cfg.room.details = _description;
    Config::cfg.room.guestLimit = _maxGuests;
    Config::cfg.room.secret = _secret;
    Config::cfg.audio.micVolume = _micVolume;
    Config::cfg.audio.micEnabled = _audioIn.isEnabled;
    Config::cfg.audio.speakersVolume = _speakersVolume;
    Config::cfg.audio.speakersEnabled = _audioOut.isEnabled;
    Config::cfg.room.latencyLimit = _latencyLimiter;
    Config::cfg.room.latencyLimitThreshold = _latencyLimit;

    // Theme
    string theme = _postThemes[_selectedTheme];
    transform(theme.begin(), theme.end(), theme.begin(), ::tolower);
    Config::cfg.room.theme = theme;

    // Artwork
    //Config::cfg.room.artworkID = Config::cfg.artwork[_selectedArtwork].id;

    Config::cfg.Save();
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
    if (_updated) {
        return true;
    } 
    return false;

    /*ParsecHostConfig cfg = _hosting.getHostConfig();

    if (
        _maxGuests != cfg.maxGuests ||
        _latencyLimiter != Config::cfg.room.latencyLimit ||
        _latencyLimit != Config::cfg.room.latencyLimitThreshold ||
        _hotseat != Config::cfg.hotseat.enabled ||
        _kioskMode != Config::cfg.kioskMode.enabled ||
        _selectedArtwork != Config::cfg.room.artworkID ||
        _gameName != Config::cfg.room.game ||
        _repThreshold != Config::cfg.room.repThreshold
    ) return true;

    return false;*/
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
