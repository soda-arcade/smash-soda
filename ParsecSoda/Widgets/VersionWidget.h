#pragma once

#include <string>
#include "../imgui/imgui.h"
#include "../globals/AppIcons.h"
#include "../globals/AppStyle.h"

class VersionWidget
{
public:
	static std::string version;

	static bool render();
	static bool renderUpdateWindow();
};

