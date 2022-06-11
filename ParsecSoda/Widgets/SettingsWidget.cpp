#include "SettingsWidget.h"

SettingsWidget::SettingsWidget(Hosting& hosting)
    : _hosting(hosting)
{
    _basicVersion = MetadataCache::preferences.basicVersion;
    _disableMicrophone = MetadataCache::preferences.disableMicrophone;
    _disableGuideButton = MetadataCache::preferences.disableGuideButton;
    _latencyLimitEnabled = MetadataCache::preferences.latencyLimitEnabled;
    _latencyLimitValue = MetadataCache::preferences.latencyLimitValue;
}

bool SettingsWidget::render()
{
    AppStyle::pushTitle();
    ImGui::SetNextWindowSizeConstraints(ImVec2(350, 150), ImVec2(500, 600));
    ImGui::Begin("Settings");
    AppStyle::pop();

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