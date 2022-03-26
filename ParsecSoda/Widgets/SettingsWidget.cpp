#include "SettingsWidget.h"

SettingsWidget::SettingsWidget(Hosting& hosting)
    : _hosting(hosting)
{
    _basicVersion = MetadataCache::preferences.basicVersion;
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
    }
    AppStyle::pop();

    ImGui::End();
    return true;
}