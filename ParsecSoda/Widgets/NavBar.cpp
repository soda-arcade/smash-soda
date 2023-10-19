#include "NavBar.h"

void NavBar::render(
	Hosting& hosting,
	bool& showLogin,
	bool& showHostSettings,
	bool& showGamepads,
	bool& showMasterOfPuppets,
	bool& showChat,
	bool& showGuests,
	bool& showLog,
	bool& showAudio,
	bool& showVideo,
	bool& showInfo,
	bool& showSettings,
	bool& showButtonLock,
	bool& showLibrary,
	bool& showOverlay,
	bool& showHotseat,
	bool& showTournament
)
{
	static ImVec2 iconSize = ImVec2(24, 24);
	static ImVec2 windowSize = ImVec2(32+4, 16*30+4);
	static ImVec2 zero = ImVec2(0, 0);
	static ImVec2 padding = ImVec2(8, 8);
	
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(2,2));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0,0));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
	ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 0);

	ImGui::Begin(
		"##NavBar", (bool*)0,
		ImGuiWindowFlags_NoDecoration |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoNav |
		ImGuiWindowFlags_NoBringToFrontOnFocus
	);
	ImGui::SetWindowSize(windowSize);
	ImGui::SetWindowPos(zero);
	
	if (ToggleIconButtonWidget::render(AppIcons::play, AppIcons::play, showHostSettings, iconSize)) {
		showHostSettings = !showHostSettings;
		MetadataCache::preferences.showHostSettings = showHostSettings;
	}
	renderNavtooltip("Host settings", showHostSettings);

	if (ToggleIconButtonWidget::render(AppIcons::padOn, AppIcons::padOn, showGamepads, iconSize)) {
		showGamepads = !showGamepads;
		MetadataCache::preferences.showGamepads = showGamepads;
	}
	renderNavtooltip("Gamepads", showGamepads);

	if (ToggleIconButtonWidget::render(AppIcons::buttonLock, AppIcons::buttonLock, showButtonLock, iconSize)) {
		showButtonLock = !showButtonLock;
		MetadataCache::preferences.showButtonLock = showButtonLock;
	}
	renderNavtooltip("Button Lock", showButtonLock);

	if (ToggleIconButtonWidget::render(AppIcons::puppet, AppIcons::puppet, showMasterOfPuppets, iconSize))
	{
		showMasterOfPuppets = !showMasterOfPuppets;
		MetadataCache::preferences.showMasterOfPuppets = showMasterOfPuppets;
	}
	renderNavtooltip("Master of Puppets", showMasterOfPuppets);

	if (ToggleIconButtonWidget::render(AppIcons::chat, AppIcons::chat, showChat, iconSize)) {
		showChat = !showChat;
		MetadataCache::preferences.showChat = showChat;
	}
	renderNavtooltip("Chat", showChat);

	if (ToggleIconButtonWidget::render(AppIcons::users, AppIcons::users, showGuests, iconSize)) {
		showGuests = !showGuests;
		MetadataCache::preferences.showGuests = showGuests;
	}
	renderNavtooltip("Guests", showGuests);

	if (ToggleIconButtonWidget::render(AppIcons::library, AppIcons::library, showLibrary, iconSize)) {
		showLibrary = !showLibrary;
		MetadataCache::preferences.showLibrary = showLibrary;
	}
	renderNavtooltip("Library", showLibrary);

	/*if (ToggleIconButtonWidget::render(AppIcons::overlay, AppIcons::overlay, showOverlay, iconSize))	showOverlay = !showOverlay;
	renderNavtooltip("Overlay", showOverlay);*/

	if (ToggleIconButtonWidget::render(AppIcons::hotseat, AppIcons::hotseat, showHotseat, iconSize)) {
		showHotseat = !showHotseat;
		MetadataCache::preferences.showHotseat = showHotseat;
	}
	renderNavtooltip("Hotseat", showHotseat);

	/*if (ToggleIconButtonWidget::render(AppIcons::tournament, AppIcons::tournament, showTournament, iconSize))	showTournament = !showTournament;
	renderNavtooltip("Tournament", showTournament);*/

	if (ToggleIconButtonWidget::render(AppIcons::log, AppIcons::log, showLog, iconSize)) showLog = !showLog;
	renderNavtooltip("Log", showLog);

	if (ToggleIconButtonWidget::render(AppIcons::speakersOn, AppIcons::speakersOn, showAudio, iconSize)) {
		showAudio = !showAudio;
		MetadataCache::preferences.showAudio = showAudio;
	}
	renderNavtooltip("Audio", showAudio);

	if (ToggleIconButtonWidget::render(AppIcons::video, AppIcons::video, showVideo, iconSize)) {
		showVideo = !showVideo;
		MetadataCache::preferences.showVideo = showVideo;
	}
	renderNavtooltip("Video", showVideo);

	if (ToggleIconButtonWidget::render(AppIcons::settings, AppIcons::settings, showSettings, iconSize)) {
		showSettings = !showSettings;
		MetadataCache::preferences.showSettings = showSettings;
	}
	renderNavtooltip("Settings", showSettings);

	if (ToggleIconButtonWidget::render(AppIcons::info, AppIcons::info, showInfo, iconSize)) showInfo = !showInfo;
	renderNavtooltip("About", showInfo);


	if (IconButton::render(AppIcons::logoff, AppColors::primary, iconSize))
	{
		if (!hosting.isRunning())
		{
			showLogin = !showLogin;
		}
	}
	TitleTooltipWidget::render("Log off", "Go back to log in screen.\n\n * Stop streaming before trying to log out.");

	ImGui::End();

	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
}

void NavBar::renderNavtooltip(const char* title, bool& show)
{
	TitleTooltipWidget::render(title, show ? "Window is visible." : "Window is hidden.");
}
