#pragma once

#include "../imgui/imgui.h"
#include "../Hosting.h"
#include "../MetadataCache.h"
#include "../Helpers/Debouncer.h"
#include "../globals/AppIcons.h"
#include "../globals/AppFonts.h"
#include "../globals/AppColors.h"
#include "../globals/AppStyle.h"
#include "TitleTooltipWidget.h"
#include "TooltipWidget.h"
#include "IntRangeWidget.h"
#include "../ButtonLock.h"

class ButtonLockWidget
{
public:
	ButtonLockWidget(Hosting& hosting);
	LockedGamepadState lockedGamepad;
	ImVec2 smallButton{ 25, 25 };
	ImVec2 wideButton{ 60, 25 };
	ImVec2 ultrawideButton{ 125, 25 };
	ImVec2 bigButton{ 60, 55 };
	void setColor(float hue = 0.35f);
	void makeButton(string buttonText, ImVec2 size, uint16_t bitwise);
	void makeButton(string buttonText, ImVec2 size, bool& boolToChange);
	void makeButton(string buttonText, ImVec2 size, bool& boolToChange1, bool& boolToChange2);
	bool render();

private:
	// Dependency injection
	Hosting& _hosting;
};
