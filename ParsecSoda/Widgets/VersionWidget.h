#pragma once

#include <string>
#include <thread>
#include "../imgui/imgui.h"
#include "../ImGui/imgui_markdown.h"
#include "../globals/AppIcons.h"
#include "../globals/AppStyle.h"

#include <nlohmann/json.hpp>
using namespace std;
using json = nlohmann::json;

class VersionWidget
{
public:
	bool showUpdate = false;
	std::string version;
	std::string latestVersion;
	std::string changeLog;

	ImGui::MarkdownConfig mdConfig;
	
	// Constructor
	VersionWidget();

	void update(std::string jsonString);
	bool render();
	bool renderUpdateWindow();
};

