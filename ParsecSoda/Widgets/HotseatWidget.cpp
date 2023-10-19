#include "HotseatWidget.h"
#include "../Hosting.h"
#include "../Modules/Hotseat.h"
extern Hosting g_hosting;

HotseatWidget::HotseatWidget(Hosting& hosting)
    : _hosting(hosting)
{
}

bool HotseatWidget::render() {

    AppStyle::pushTitle();
    ImGui::SetNextWindowSizeConstraints(ImVec2(150, 150), ImVec2(800, 900));
    ImGui::Begin("Hotseat", (bool*)0);
    AppStyle::pushInput();

    ImVec2 size = ImGui::GetContentRegionAvail();

    ImGui::BeginChild("Hotseat List", ImVec2(size.x, size.y));

    AppStyle::pushLabel();
    AppStyle::pop();
    ImGui::SetNextItemWidth(size.x - 42);
    ImGui::SameLine();
    ImGui::Dummy(ImVec2(0, 5));

    if (ImGui::BeginTabBar("Hotseat Tabs", ImGuiTabBarFlags_None))
    {
        AppFonts::pushInput();
        AppColors::pushTitle();
        if (ImGui::BeginTabItem("Overview"))
        {
            renderOverview();
            ImGui::EndTabItem();
        }
        AppColors::pushTitle();
        if (ImGui::BeginTabItem("Settings")) {
            renderSettings();
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

void HotseatWidget::renderOverview() {

	Hotseat& hotseat = g_hosting.getHotseat();
    static ImVec2 size;
    static ImVec2 pos;
    size = ImGui::GetContentRegionAvail();
    pos = ImGui::GetWindowPos();

    ImGui::Dummy(ImVec2(0, 10.0f));

	//AppStyle::pushLabel();
	//ImGui::Text("Time Remaining");
    //AppStyle::pushPositive();
	//string time = hotseat.formatDuration(hotseat.playTimer.getRemainingTime());
	//ImGui::Text(time.c_str());

    ImGui::Dummy(ImVec2(0, 10.0f));
    
    AppStyle::pushLabel();
    ImGui::Text("Queue");
    ImGui::Separator();

    ImGui::Dummy(ImVec2(0, 5.0f));
    
	for (int i = 0; i < hotseat.queue.size(); i++) {
        AppStyle::pushPositive();
        string index = to_string(i + 1) + ". ";
        ImGui::Text(index.c_str());
		ImGui::SameLine();
        
        if (i < MetadataCache::preferences.hotseatSeats) {
            AppStyle::pushPositive();
        }
        else {
            AppStyle::pushInput();
        }
		ImGui::Text(hotseat.queue[i].name.c_str());

        // Seat time
		if (i < MetadataCache::preferences.hotseatSeats) {
            AppStyle::pushNegative();
            string time = hotseat.formatDuration(hotseat.seats[i].timer.getRemainingTime());
			ImGui::Text(time.c_str());
		}
        
	}

    AppStyle::pop();
    AppStyle::pushInput();
    
}

void HotseatWidget::renderSettings() {

    _hotseatSeats = MetadataCache::preferences.hotseatSeats;
    _hotseatTime = MetadataCache::preferences.hotseatTime;
    _hotseatAFK = MetadataCache::preferences.hotseatAFK;
    _hotseatAFKTime = MetadataCache::preferences.hotseatAFKTime;
    _hotseatPause = MetadataCache::preferences.hotseatPause;

    static ImVec2 size;
    static ImVec2 pos;
    size = ImGui::GetContentRegionAvail();
    pos = ImGui::GetWindowPos();

    ImGui::Dummy(ImVec2(0, 10.0f));

    if (ImForm::InputNumber("SEATS", _hotseatSeats, 1, 8,
        "The number of guests that can be put in to hotseat(s).")) {
        MetadataCache::preferences.hotseatSeats = _hotseatSeats;
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

    AppStyle::pop();
    AppStyle::pushInput();

}