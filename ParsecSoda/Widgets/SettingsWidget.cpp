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
    ImGui::SetNextWindowSizeConstraints(ImVec2(350, 150), ImVec2(500, 600));
    ImGui::Begin("Settings");
    AppStyle::pop();

    AppStyle::pushLabel();
    ImGui::Text("THEME");
    AppStyle::pushInput();
    string themes[4] = { "Midnight", "Parsec Soda", "Parsec Soda V", "Mini" };
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
    ImGui::Text("You will need to restart to see your changes.");
    AppStyle::pop();

    ImGui::Dummy(ImVec2(0, 10.0f));
    AppStyle::pushLabel();
    ImGui::Text("DISCORD INVITE LINK");
    AppStyle::pushInput();
    if (ImGui::InputText("##Secret input", _discord, HOST_NAME_LEN)) {

        MetadataCache::preferences.discord = _discord;
        MetadataCache::savePreferences();

    }

    ImGui::Dummy(ImVec2(0, 10.0f));

    AppStyle::pushLabel();
    ImGui::Text("CHATBOT NAME");
    AppStyle::pushInput();
    if (ImGui::InputText("##Chatbot input", _chatbot, HOST_NAME_LEN)) {

        MetadataCache::preferences.chatbot = _chatbot;
        MetadataCache::savePreferences();

    }

    ImGui::Dummy(ImVec2(0, 20.0f));

    AppStyle::pushInput();
    if (ImGui::Checkbox("Disable !sfx & !bonk (Basic version)", &_basicVersion))
    {
        MetadataCache::preferences.basicVersion = _basicVersion;
        MetadataCache::savePreferences();
        _hosting.getChatBot()->updateSettings();
    }
    AppStyle::pop();

    AppStyle::pushInput();
    if (ImGui::Checkbox("Disable Microphone", &_disableMicrophone))
    {
        MetadataCache::preferences.disableMicrophone = _disableMicrophone;
        MetadataCache::savePreferences();
        _hosting._disableMicrophone = _disableMicrophone;
    }
    AppStyle::pop();

    AppStyle::pushInput();
    if (ImGui::Checkbox("Disable Guide Button", &_disableGuideButton))
    {
        MetadataCache::preferences.disableGuideButton = _disableGuideButton;
        MetadataCache::savePreferences();
        _hosting._disableGuideButton = _disableGuideButton;
    }
    AppStyle::pop();

    AppStyle::pushInput();
    if (ImGui::Checkbox("Disable Keyboards", &_disableKeyboard))
    {
        MetadataCache::preferences.disableKeyboard = _disableKeyboard;
        MetadataCache::savePreferences();
        _hosting._disableKeyboard = _disableKeyboard;
    }
    AppStyle::pop();

    AppStyle::pushInput();
    if (ImGui::Checkbox("Enable Leaderboard", &_leaderboardEnabled)) {
        MetadataCache::preferences.leaderboardEnabled = _leaderboardEnabled;
        MetadataCache::savePreferences();
    }
    AppStyle::pop();

    // Test
    //AppStyle::pushInput();
    //if (ImGui::Checkbox("Latency limit", &_latencyLimitEnabled))
    //{
    //    MetadataCache::preferences.latencyLimitEnabled = _latencyLimitEnabled;
    //    MetadataCache::savePreferences();
    //    _hosting._latencyLimitEnabled = _latencyLimitEnabled;
    //}
    //AppStyle::pop();

    //ImGui::SameLine();

    //static int latencyLimit = _latencyLimitValue;
    //static int previousLatencyLimit;
    //previousLatencyLimit = latencyLimit;

    //if (IntRangeWidget::render("latency limit", latencyLimit, 50, 300))
    //{
    //    TitleTooltipWidget::render("Latency Limit", "Kick user if their average latency stays above this");
    //}
    //static Debouncer debouncer(DEBOUNCER_TIME_MS, [&]() {
    //    MetadataCache::preferences.latencyLimitValue = latencyLimit;
    //    MetadataCache::savePreferences();
    //    _hosting._latencyLimitValue = latencyLimit;
    //});
    //if (latencyLimit != previousLatencyLimit)
    //    debouncer.start();

    ImGui::End();
    return true;
}