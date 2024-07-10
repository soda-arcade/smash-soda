#include "HostSettingsWidget.h"
#include "../ImGui/imform.h"

HostSettingsWidget::HostSettingsWidget(Hosting& hosting, function<void(bool)> onHostRunningStatusCallback)
    : _hosting(hosting), _audioIn(_hosting.audioIn), _audioOut(_hosting.audioOut), _onHostRunningStatusCallback(onHostRunningStatusCallback)
{
    ParsecHostConfig cfg = hosting.getHostConfig();
    try
    {
        strcpy_s(_gameID, cfg.gameID);
        strcpy_s(_gameName, Config::cfg.room.game.c_str());
		strcpy_s(_description, Config::cfg.room.details.c_str());
        strcpy_s(_secret, cfg.secret);
        strcpy_s(_kioskApplication, "");
        strcpy_s(_kioskParam, "");
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
        } catch (const std::exception&) {}
    }
    _maxGuests = cfg.maxGuests;
    
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

    // Game must be between 3 and 255 characters
	if (strlen(_gameName) < 3 || strlen(_gameName) > 255) {
		_gameNameError = "Game name must be between 3 and 255 characters.";
		return false;
	}

	// Details can't be longer than 255 characters
	if (strlen(_description) > 255) {
		_descriptionError = "Description can't be longer than 255 characters.";
		return false;
	}

	return true;
    
}

/// <summary>
/// Renders the host settings widget.
/// </summary>
bool HostSettingsWidget::render(HWND& hwnd) {
    
    AppStyle::pushTitle();
    ImGui::SetNextWindowPos(ImVec2(464, 5), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSizeConstraints(ImVec2(500, 500), ImVec2(600, 700));
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
    if (_gameNameError.length() > 0) {
        AppStyle::pushInput();
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
		ImGui::TextWrapped(_gameNameError.c_str());
    }

    if (!Config::cfg.room.privateRoom) {
        if (ImForm::InputText("DETAILS", _description,
            "Any additional details about the game or your room. This will be displayed when somebody clicks your post.")) {

            // Can't be longer than 255 characters
            if (strlen(_description) > 255) {
                string desc = _description;
                desc = desc.substr(0, 255);
                strcpy_s(_description, desc.c_str());
			}

        }
        if (_descriptionError.length() > 0) {
            AppStyle::pushInput();
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
            ImGui::TextWrapped(_descriptionError.c_str());
        }

        if (Arcade::instance.artwork.size() > 0) {
            AppStyle::pushLabel();
            ImGui::Text("CUSTOM ARTWORK");
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
                    }
                    if (isSelected) {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
            AppStyle::pushLabel();
            ImGui::TextWrapped("If you have any custom artwork uploaded to Soda Arcade, you can select it here.");
            AppStyle::pop();
            ImGui::Dummy(ImVec2(0, 10.0f));
        }

        AppStyle::pushLabel();
        ImGui::Text("POST THEME");
        AppStyle::pushInput();
        ImGui::SetNextItemWidth(size.x);
        if (ImGui::BeginCombo("### Post theme picker combo", _postThemes[_selectedTheme].c_str(), ImGuiComboFlags_HeightLarge)) {

            for (size_t i = 0; i < 6; ++i) {
                bool isSelected = (i == _selectedTheme);
                if (ImGui::Selectable(_postThemes[i].c_str(), isSelected)) {
                    _selectedTheme = i;
                }
                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
        AppStyle::pushLabel();
        ImGui::TextWrapped("This lets you change the colour theme of your room poster on Soda Arcade.");
        AppStyle::pop();
        ImGui::Dummy(ImVec2(0, 10.0f));

    }

    char link[256];
    if (Config::cfg.room.privateRoom) {
        strcpy_s(link, _secretLink);
	}
    else {
		string arcadeLink = "https://soda-arcade.com/invite/" + Config::cfg.arcade.username + "/room";
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

    ImGui::BeginChild("##Guest slot child", ImVec2(size.x / 3, 50.0f));
    ImGui::Text("GUEST SLOTS");
    if (IntRangeWidget::render("guest count", _maxGuests, 0, 20, 0.025f)) {
		Config::cfg.room.guestLimit = _maxGuests;
    }
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("##Public room child", ImVec2(size.x / 3, 50.0f));
    ImGui::Text("PUBLIC ROOM");
    ImGui::Indent(20);
    if (ToggleIconButtonWidget::render(AppIcons::yes, AppIcons::no, !Config::cfg.room.privateRoom, AppColors::positive, AppColors::negative, ImVec2(22, 22))) {

		// If public game is false, check we have authenticated with Soda Arcade
		if (Config::cfg.room.privateRoom) {
			if (Config::cfg.arcade.token == "") {
				Config::cfg.arcade.showLogin = true;
			}
			else {
				Config::cfg.room.privateRoom = false;
			}
		}
		else {
			Config::cfg.room.privateRoom = true;
		}

        //_publicGame = !_publicGame;
        //Config::cfg.room.publicRoom = _publicGame;
        //updateSecretLink();
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
        if (!Config::cfg.kioskMode.enabled && _libraryID == -1) {
            return;
        }
        
        Config::cfg.kioskMode.enabled = !Config::cfg.kioskMode.enabled;
        if (_hosting.isRunning()) {
            if (Config::cfg.kioskMode.enabled && _libraryID != -1) {
                ProcessMan::instance.start(games[_libraryID].path, games[_libraryID].parameters);
			} else {
				ProcessMan::instance.stop();
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
        if (_hosting.isRunning()) {
            _hosting.stopHosting();
            if (_onHostRunningStatusCallback != nullptr) _onHostRunningStatusCallback(false);
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
				ProcessMan::instance.start(games[_libraryID].path, games[_libraryID].parameters);
			}
        }
    }

    if (_hosting.isRunning()) {

        ImGui::Dummy(dummySize);
        indentSize = 0.5f * size.x - 65.0f;
        ImGui::Indent(indentSize);

        AppColors::pushInput();
        AppFonts::pushTitle();
        ImGui::PushStyleColor(ImGuiCol_Button, AppColors::primary);
        
        // Parsec settings were changed
        if (isDirty()) {

            if (ImGui::Button("Update Settings")) {
                // Apply settings for Parsec
                _hosting.setHostConfig("", "1wdoHfhhZH5lPuZCwGBete0HIAj", _maxGuests, false, _secret);
                _hosting.applyHostConfig();

                savePreferences();

                // Update on Soda Arcade
                if (!Config::cfg.room.privateRoom) {
                    Arcade::instance.createPost();
                }
            }
            TitleTooltipWidget::render("Update Room Settings", "The room will be instantly updated with your new settings.");

        } else 

        // Soda Arcade settings were changed
        if (_gameName != Config::cfg.room.game || _description != Config::cfg.room.details) {
            if (ImGui::Button("Update Settings")) {
                savePreferences();

                // Update on Soda Arcade
                if (!Config::cfg.room.privateRoom) {
                    Arcade::instance.createPost();
                }
            }
            TitleTooltipWidget::render("Update Room Settings", "The room will be instantly updated with your new settings.");
        }

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
        if (Config::cfg.audio.micEnabled) _audioIn.captureAudio();
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

    if (Config::cfg.audio.micEnabled)
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
    Config::cfg.room.artworkID = Config::cfg.artwork[_selectedArtwork].id;

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
    ParsecHostConfig cfg = _hosting.getHostConfig();

    if (
        _publicGame != cfg.publicGame ||
        _maxGuests != cfg.maxGuests ||
        _latencyLimiter != Config::cfg.room.latencyLimit ||
        _latencyLimit != Config::cfg.room.latencyLimitThreshold ||
        (strcmp("", cfg.name) != 0) ||
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
