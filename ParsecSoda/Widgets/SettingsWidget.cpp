#include "SettingsWidget.h"
#include "../ImGui/imform.h"

SettingsWidget::SettingsWidget(Hosting& hosting)
    : _hosting(hosting)
{
    _basicVersion = MetadataCache::preferences.basicVersion;
    _disableMicrophone = MetadataCache::preferences.disableMicrophone;
    _disableGuideButton = MetadataCache::preferences.disableGuideButton;
    _disableKeyboard = MetadataCache::preferences.disableKeyboard;
    _latencyLimitEnabled = MetadataCache::preferences.latencyLimitEnabled;
    _latencyLimitValue = MetadataCache::preferences.latencyLimitValue;
    _leaderboardEnabled = MetadataCache::preferences.leaderboardEnabled;

    _muteTime = MetadataCache::preferences.muteTime;
    _autoMute = MetadataCache::preferences.autoMute;
    _autoMuteTime = MetadataCache::preferences.autoMuteTime;
    _saveChat = MetadataCache::preferences.saveLog;

    try
    {
        strcpy_s(_discord, MetadataCache::preferences.discord.c_str());
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
        strcpy_s(_chatbot, MetadataCache::preferences.chatbot.c_str());
    }
    catch (const std::exception&) {
        try
        {
            strcpy_s(_chatbot, "");
        }
        catch (const std::exception&) {}
    }
}

bool SettingsWidget::render()
{
    AppStyle::pushTitle();
    ImGui::SetNextWindowSizeConstraints(ImVec2(150, 150), ImVec2(800, 900));
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
    string themes[4] = { "Midnight", "Parsec Soda", "Parsec Soda V", "Mini" };
    ImGui::SetNextItemWidth(size.x);
    if (ImGui::BeginCombo("### Thumbnail picker combo", themes[MetadataCache::preferences.theme].c_str(), ImGuiComboFlags_HeightLarge)) {
        for (size_t i = 0; i < 4; ++i) {
            bool isSelected = (i == MetadataCache::preferences.theme);
            if (ImGui::Selectable(themes[i].c_str(), isSelected)) {
                MetadataCache::saveTheme(i);
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

    if (ImForm::InputCheckbox("Disable Microphone", _disableMicrophone,
        "When enabled, the microphone cause audio issues in some games.")) {
        MetadataCache::preferences.disableMicrophone = _disableMicrophone;
        MetadataCache::savePreferences();
        _hosting._disableMicrophone = _disableMicrophone;
    }

    if (ImForm::InputCheckbox("Disable Guide Button", _disableGuideButton,
        "The guide button by default often brings up overlays in software, which can cause issues when hosting.")) {
        MetadataCache::preferences.disableGuideButton = _disableGuideButton;
        MetadataCache::savePreferences();
        _hosting._disableGuideButton = _disableGuideButton;
    }

    if (ImForm::InputCheckbox("Disable Keyboard", _disableKeyboard,
        "Prevents users without gamepads playing with keyboard.")) {
        MetadataCache::preferences.disableKeyboard = _disableKeyboard;
        MetadataCache::savePreferences();
        _hosting._disableKeyboard = _disableKeyboard;
    }

    AppStyle::pop();
    AppStyle::pushInput();

}

/// <summary>
/// Renders the chatbot options tab.
/// </summary>
void SettingsWidget::renderChatbot() {

    ImGui::Dummy(ImVec2(0, 10.0f));

    if (ImForm::InputCheckbox("Disable !sfx & !bonk", _basicVersion,
        "Prevents guests from using these commands in chat.")) {
        MetadataCache::preferences.basicVersion = _basicVersion;
        MetadataCache::savePreferences();
        _hosting.getChatBot()->updateSettings();
    }

    if (ImForm::InputText("CHATBOT NAME", _chatbot,
        "Can give the ChatBot a silly name if you want!")) {
        MetadataCache::preferences.chatbot = _chatbot;
        MetadataCache::savePreferences();
    }

    if (ImForm::InputText("DISCORD INVITE LINK", _discord,
        "Automatically print invite link in chat with !discord")) {
        MetadataCache::preferences.discord = _discord;
        MetadataCache::savePreferences();
    }

    if (ImForm::InputNumber("MUTE TIME", _muteTime, 5, 60,
        "How long a person stays gagged with the !mute command")) {
        MetadataCache::preferences.muteTime = _muteTime;
        MetadataCache::savePreferences();
    }

    if (ImForm::InputCheckbox("Auto Mute", _autoMute,
        "Automatically mutes a guest if they send a lot of messages quickly")) {
        MetadataCache::preferences.autoMute = _autoMute;
        MetadataCache::savePreferences();
    }

    if (ImForm::InputNumber("AUTO MUTE TIME", _autoMuteTime, 5, 5000,
        "Here you can set the the auto mute time between messages (in ms). Set this to a low value.")) {
        MetadataCache::preferences.autoMuteTime = _autoMuteTime;
        MetadataCache::savePreferences();
    }

    /*
    if (ImForm::InputCheckbox("Save Log", _saveChat,
        "Logs are stored in SmashSoda appdata folder")) {
        MetadataCache::preferences.saveLog = _saveLog;
        MetadataCache::savePreferences();
    }
    */

}