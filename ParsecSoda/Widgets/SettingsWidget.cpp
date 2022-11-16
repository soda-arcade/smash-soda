#include "SettingsWidget.h"

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
        if (ImGui::BeginTabItem("Chatbot"))
        {
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

    AppStyle::pushInput();
    if (ImGui::Checkbox("Disable !sfx & !bonk (Basic version)", &_basicVersion))
    {
        MetadataCache::preferences.basicVersion = _basicVersion;
        MetadataCache::savePreferences();
        _hosting.getChatBot()->updateSettings();
    }
    AppStyle::pushLabel();
    ImGui::TextWrapped("Prevents guests from using these commands in chat.");
    AppStyle::pop();

    AppStyle::pushInput();
    if (ImGui::Checkbox("Disable Microphone", &_disableMicrophone))
    {
        MetadataCache::preferences.disableMicrophone = _disableMicrophone;
        MetadataCache::savePreferences();
        _hosting._disableMicrophone = _disableMicrophone;
    }
    AppStyle::pushLabel();
    ImGui::TextWrapped("When enabled, the microphone cause audio issues in some games.");
    AppStyle::pop();

    AppStyle::pushInput();
    if (ImGui::Checkbox("Disable Guide Button", &_disableGuideButton))
    {
        MetadataCache::preferences.disableGuideButton = _disableGuideButton;
        MetadataCache::savePreferences();
        _hosting._disableGuideButton = _disableGuideButton;
    }
    AppStyle::pushLabel();
    ImGui::TextWrapped("The guide button by default often brings up overlays in software, which can cause issues when hosting.");
    AppStyle::pop();

    AppStyle::pushInput();
    if (ImGui::Checkbox("Disable Keyboards", &_disableKeyboard))
    {
        MetadataCache::preferences.disableKeyboard = _disableKeyboard;
        MetadataCache::savePreferences();
        _hosting._disableKeyboard = _disableKeyboard;
    }
    AppStyle::pushLabel();
    ImGui::TextWrapped("Prevents users without gamepads playing with keyboard.");
    AppStyle::pop();

    AppStyle::pushInput();
    if (ImGui::Checkbox("Enable Leaderboard", &_leaderboardEnabled)) {
        MetadataCache::preferences.leaderboardEnabled = _leaderboardEnabled;
        MetadataCache::savePreferences();
    }
    AppStyle::pushLabel();
    ImGui::TextWrapped("The tournament commands have a global leaderboard for your guests to keep track of wins.");

    AppStyle::pop();
    AppStyle::pushInput();

}

/// <summary>
/// Renders the chatbot options tab.
/// </summary>
void SettingsWidget::renderChatbot() {

    ImGui::Dummy(ImVec2(0, 10.0f));

    AppStyle::pushLabel();
    ImGui::Text("CHATBOT NAME");
    AppStyle::pushInput();
    if (ImGui::InputText("##Chatbot input", _chatbot, HOST_NAME_LEN)) {

        MetadataCache::preferences.chatbot = _chatbot;
        MetadataCache::savePreferences();

    }

    ImGui::Dummy(ImVec2(0, 10.0f));
    AppStyle::pushLabel();
    ImGui::Text("DISCORD INVITE LINK");
    AppStyle::pushInput();
    if (ImGui::InputText("##Secret input", _discord, HOST_NAME_LEN)) {

        MetadataCache::preferences.discord = _discord;
        MetadataCache::savePreferences();

    }

}