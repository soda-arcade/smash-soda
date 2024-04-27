#include "OverlayWidget.h"
#include "../Hosting.h"
extern Hosting g_hosting;

OverlayWidget::OverlayWidget(Hosting& hosting)
    : _hosting(hosting)
{
    _chat_enabled = Config::cfg.overlay.chat.active;
	_chat_position = Config::cfg.overlay.chat.position;
    _show_history = Config::cfg.overlay.chat.showHistory;

	_pads_enabled = Config::cfg.overlay.gamepads.active;
	_pads_position = Config::cfg.overlay.gamepads.position;

	_guests_enabled = Config::cfg.overlay.guests.active;
	_guests_position = Config::cfg.overlay.guests.position;
    _show_latency = Config::cfg.overlay.guests.showLatency;
}

bool OverlayWidget::render() {

    // Widget top
    AppStyle::pushTitle();
    ImGui::SetNextWindowSizeConstraints(ImVec2(400, 400), ImVec2(800, 900));
    ImGui::Begin("Overlay", (bool*)0);
    AppStyle::pushInput();
    
    ImVec2 size = ImGui::GetContentRegionAvail();

    if (ImForm::InputCheckbox("Show Chat", _chat_enabled,
		"Show chat window on the overlay.")) {
        Config::cfg.overlay.chat.active = _chat_enabled;
        Config::cfg.Save();
        g_hosting.getOverlay().updateSettings();
    }

    if (ImForm::InputCheckbox("Show History", _show_history,
        "This allows you to scroll through previous messages with the mouse wheel.")) {
        Config::cfg.overlay.chat.showHistory = _show_history;
        Config::cfg.Save();
        g_hosting.getOverlay().updateSettings();
    }

    AppStyle::pushLabel();
    ImGui::Text("CHAT POSITION");
    AppStyle::pop();
    ImGui::SetNextItemWidth(size.x);
    if (ImGui::BeginCombo("### Chat picker combo", _chat_position.c_str(), ImGuiComboFlags_HeightLarge)) {
        for (size_t i = 0; i < 6; ++i) {
            
            bool isSelected = false;
			if (Config::cfg.overlay.chat.position == _position_options[i].c_str()) {
				isSelected = true;
				_chat_position = _position_options[i].c_str();
			}
            
            if (ImGui::Selectable(_position_options[i].c_str(), isSelected)) {
                Config::cfg.overlay.chat.position = _position_options[i].c_str();
                _chat_position = _position_options[i].c_str();
                Config::cfg.Save();
                g_hosting.getOverlay().updateSettings();
            }
            if (isSelected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    ImGui::Dummy(ImVec2(0, 20.0f));

    if (ImForm::InputCheckbox("Show Gamepads", _pads_enabled,
        "Show gamepad info on the overlay.")) {
        Config::cfg.overlay.gamepads.active = _pads_enabled;
        Config::cfg.Save();
        g_hosting.getOverlay().updateSettings();
    }

    AppStyle::pushLabel();
    ImGui::Text("GAMEPADS POSITION");
    AppStyle::pop();
    ImGui::SetNextItemWidth(size.x);
    if (ImGui::BeginCombo("### Pads picker combo", _pads_position.c_str(), ImGuiComboFlags_HeightLarge)) {
        for (size_t i = 0; i < 6; ++i) {

            bool isSelected = false;
            if (Config::cfg.overlay.gamepads.position == _position_options[i].c_str()) {
                isSelected = true;
                _pads_position = _position_options[i].c_str();
            }

            if (ImGui::Selectable(_position_options[i].c_str(), isSelected)) {
                Config::cfg.overlay.gamepads.position = _position_options[i].c_str();
                _pads_position = _position_options[i].c_str();
                Config::cfg.Save();
                g_hosting.getOverlay().updateSettings();
            }
            if (isSelected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    ImGui::Dummy(ImVec2(0, 20.0f));

    if (ImForm::InputCheckbox("Show Guests", _guests_enabled,
        "Show guest info on the overlay.")) {
        Config::cfg.overlay.guests.active = _guests_enabled;
        Config::cfg.Save();
        g_hosting.getOverlay().updateSettings();
    }

    if (ImForm::InputCheckbox("Show Latency", _show_latency,
        "Show guests' pings on the guests widget.")) {
        Config::cfg.overlay.guests.showLatency = _show_latency;
        Config::cfg.Save();
        g_hosting.getOverlay().updateSettings();
    }

    AppStyle::pushLabel();
    ImGui::Text("GUESTS POSITION");
    AppStyle::pop();
    ImGui::SetNextItemWidth(size.x);
    if (ImGui::BeginCombo("### Guests picker combo", _guests_position.c_str(), ImGuiComboFlags_HeightLarge)) {
        for (size_t i = 0; i < 6; ++i) {

            bool isSelected = false;
            if (Config::cfg.overlay.guests.position == _position_options[i].c_str()) {
                isSelected = true;
                _guests_position = _position_options[i].c_str();
            }

            if (ImGui::Selectable(_position_options[i].c_str(), isSelected)) {
                Config::cfg.overlay.guests.position = _position_options[i].c_str();
                _guests_position = _position_options[i].c_str();
                Config::cfg.Save();
                g_hosting.getOverlay().updateSettings();
            }
            if (isSelected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    // Widget bottom
    AppStyle::pop();
    ImGui::End();
    AppStyle::pop();

    return true;

}