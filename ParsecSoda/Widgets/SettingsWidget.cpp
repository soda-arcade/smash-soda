#include "SettingsWidget.h"
#include "../ImGui/imform.h"

SettingsWidget::SettingsWidget(Hosting& hosting)
    : _hosting(hosting)
{
    _disableGuideButton = Config::cfg.input.disableGuideButton;
    _disableKeyboard = Config::cfg.input.disableKeyboard;
    _latencyLimitEnabled = Config::cfg.room.latencyLimit;
    _latencyLimitThreshold = Config::cfg.room.latencyLimitThreshold;
    _leaderboardEnabled = false;
    _autoIndex = Config::cfg.input.autoIndex;
    _flashWindow = Config::cfg.general.flashWindow;
    _sfxEnabled = Config::cfg.audio.sfxEnabled;
    _bonkEnabled = Config::cfg.chat.bonkEnabled;
    _hostBonkProof = Config::cfg.chat.hostBonkProof;
    _microphoneEnabled = Config::cfg.audio.micEnabled;

    _muteTime = Config::cfg.chat.muteTime;
    _autoMute = Config::cfg.chat.autoMute;
    _autoMuteTime = Config::cfg.chat.autoMuteTime;
    _saveChat = false;

    _prependPingLimit = false;

    try
    {
        strcpy_s(_discord, Config::cfg.chat.discord.c_str());
    }
    catch (const std::exception&)
    {
        try
        {
            strcpy_s(_discord, "");
        }
        catch (const std::exception&) {}
    }

    try {
        strcpy_s(_chatbot, Config::cfg.chat.chatbot.c_str());
    }
    catch (const std::exception&) {
        try
        {
            strcpy_s(_chatbot, "");
        }
        catch (const std::exception&) {}
    }

    try {
        strcpy_s(_welcomeMessage, Config::cfg.chat.welcomeMessage.c_str());
    }
    catch (const std::exception&) {
        try
        {
            strcpy_s(_welcomeMessage, "");
        }
        catch (const std::exception&) {}
    }
}

bool SettingsWidget::render()
{
    AppStyle::pushTitle();
    ImGui::SetNextWindowSizeConstraints(ImVec2(400, 400), ImVec2(800, 900));
    ImGui::Begin("Settings", (bool*)0);
    AppStyle::pushInput();

    ImVec2 size = ImGui::GetContentRegionAvail();

    ImGui::BeginChild("Settings List", ImVec2(size.x, size.y));

    AppStyle::pushLabel();
    AppStyle::pop();
    ImGui::SetNextItemWidth(size.x - 42);
    ImGui::SameLine();
    ImGui::Dummy(ImVec2(0, 5));

    if (ImGui::BeginTabBar("Settings Tabs", ImGuiTabBarFlags_None))
    {
        AppFonts::pushInput();
        AppColors::pushTitle();
        if (ImGui::BeginTabItem("General"))
        {
            renderGeneral();
            ImGui::EndTabItem();
        }
        AppColors::pushTitle();
        if (ImGui::BeginTabItem("Chat")) {
            renderChatbot();
            ImGui::EndTabItem();
        }
        AppColors::pushTitle();
        if (ImGui::BeginTabItem("Permissions")) {
            renderPermissions();
            ImGui::EndTabItem();
        }
        /*AppColors::pushTitle();
        if (ImGui::BeginTabItem("Log")) {
            renderLog();
            ImGui::EndTabItem();
        }*/
        AppColors::pop();
        AppFonts::pop();
        ImGui::EndTabBar();
    }

    ImGui::EndChild();

    AppStyle::pop();
    ImGui::End();
    AppStyle::pop();

    return true;
}

/// <summary>
/// Renders the general settings tab.
/// </summary>
void SettingsWidget::renderGeneral() {

    static ImVec2 size;
    static ImVec2 pos;
    size = ImGui::GetContentRegionAvail();
    pos = ImGui::GetWindowPos();

    ImGui::Dummy(ImVec2(0, 10.0f));

    AppStyle::pushLabel();
    ImGui::Text("THEME");
    AppStyle::pushInput();
    ImGui::SetNextItemWidth(size.x);
    if (ImGui::BeginCombo("### Thumbnail picker combo", themes[Config::cfg.general.theme].c_str(), ImGuiComboFlags_HeightLarge)) {
        for (size_t i = 0; i < 5; ++i) {
            bool isSelected = (i == Config::cfg.general.theme);
            if (ImGui::Selectable(themes[i].c_str(), isSelected)) {
				Config::cfg.general.theme = i;
				Config::cfg.Save();
            }
            if (isSelected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
    AppStyle::pushLabel();
    ImGui::TextWrapped("You will need to restart to see your changes.");
    AppStyle::pop();

    ImGui::Dummy(ImVec2(0, 20.0f));

    if (ImForm::InputCheckbox("Disable Microphone", _microphoneEnabled,
        "When enabled, the microphone cause audio issues in some games.")) {
        Config::cfg.audio.micEnabled = _microphoneEnabled;
		Config::cfg.Save();
    }

    if (ImForm::InputCheckbox("Disable Guide Button", _disableGuideButton,
        "The guide button by default often brings up overlays in software, which can cause issues when hosting.")) {
        Config::cfg.input.disableGuideButton = _disableGuideButton;
        Config::cfg.Save();
        _hosting._disableGuideButton = _disableGuideButton;
    }

    if (ImForm::InputCheckbox("Disable Keyboard", _disableKeyboard,
        "Prevents users without gamepads playing with keyboard.")) {
        Config::cfg.input.disableKeyboard = _disableKeyboard;
        Config::cfg.Save();
        _hosting._disableKeyboard = _disableKeyboard;
    }

    if (ImForm::InputCheckbox("Auto Index Gamepads", _autoIndex,
        "XInput indices will be identified automatically. Beware, this may cause BSOD crashes for some users!")) {
        Config::cfg.input.autoIndex = _autoIndex;
        Config::cfg.Save();
    }

    AppStyle::pop();
    AppStyle::pushInput();

}

/// <summary>
/// Renders the chatbot options tab.
/// </summary>
void SettingsWidget::renderChatbot() {

    ImGui::Dummy(ImVec2(0, 10.0f));

    if (ImForm::InputCheckbox("Disable !sfx", _sfxEnabled,
        "Prevents guests from using these commands in chat.")) {
        Config::cfg.audio.sfxEnabled = _sfxEnabled;
        Config::cfg.Save();
    }

    if (ImForm::InputText("CHATBOT NAME", _chatbot,
        "Can give the ChatBot a silly name if you want!")) {
        Config::cfg.chat.chatbot = _chatbot;
        Config::cfg.Save();
        Config::cfg.chatbotName = "[" + Config::cfg.chat.chatbot + "] ";
    }

    if (ImForm::InputTextArea("WELCOME MESSAGE", _welcomeMessage,
        "Joining guests will see this message. Type _PLAYER_ to insert the guest's name in the message.")) {
        Config::cfg.chat.welcomeMessage = _welcomeMessage;
        Config::cfg.Save();
    }

    if (ImForm::InputText("DISCORD INVITE LINK", _discord,
        "Automatically print invite link in chat with !discord")) {
        Config::cfg.chat.discord = _discord;
        Config::cfg.Save();
    }

    if (ImForm::InputCheckbox("Flash Window on Message", _flashWindow,
        "The window will flash when a message is received, when not focused.")) {
        Config::cfg.general.flashWindow = _flashWindow;
        Config::cfg.Save();
    }

    if (ImForm::InputCheckbox("Disable !bonk", _bonkEnabled,
        "Funny at first, but can quickly turn annoying.")) {
        Config::cfg.chat.bonkEnabled = _bonkEnabled;
        Config::cfg.Save();
    }

    /*if (ImForm::InputCheckbox("Host can't be bonked", _hostBonkProof,
        "You DARE bonk the host!?")) {
        Config::cfg.chat.hostBonkProof = _hostBonkProof;
        Config::cfg.Save();
    }*/

}

/// <summary>
/// Render log settings.
/// </summary>
void SettingsWidget::renderLog() {
	// TODO: Add log settings
}

/// <summary>
/// Renders the chatbot options tab.
/// </summary>
void SettingsWidget::renderPermissions() {

    ImGui::Dummy(ImVec2(0, 10.0f));

    AppStyle::pushTitle();
    ImGui::Text("Regular Guest");

    if (ImForm::InputCheckbox("Can use !sfx command", _guestSFX)) {
        Config::cfg.permissions.guest.useSFX = _guestSFX;
        Config::cfg.Save();
    }

    if (ImForm::InputCheckbox("Can use !bb command", _guestBB)) {
        Config::cfg.permissions.guest.useBB = _guestBB;
        Config::cfg.Save();
    }

    AppStyle::pushTitle();
    ImGui::Text("VIPs");

    if (ImForm::InputCheckbox("Can use !bb command", _vipBB)) {
        Config::cfg.permissions.vip.useBB = _vipBB;
        Config::cfg.Save();
    }

    AppStyle::pushTitle();
    ImGui::Text("Moderators");

    if (ImForm::InputCheckbox("Can use !sfx command", _modSFX)) {
        Config::cfg.permissions.moderator.useSFX = _modSFX;
        Config::cfg.Save();
    }

    if (ImForm::InputCheckbox("Can use !bb command", _modBB)) {
        Config::cfg.permissions.moderator.useBB = _modBB;
        Config::cfg.Save();
    }

}