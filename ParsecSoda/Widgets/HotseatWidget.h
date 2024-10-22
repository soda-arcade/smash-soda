#pragma once
#pragma once

#include "../ImGui/imgui.h"
#include "../Hosting.h"
#include "../globals/AppIcons.h"
#include "../globals/AppStyle.h"
#include "IconButton.h"
#include "TitleTooltipWidget.h"
#include "ConfirmPopupWidget.h"
#include "PopupWidgetEdit.h"
#include "../ImGui/imform.h"

class HotseatWidget {
public:

	HotseatWidget(Hosting& hosting);
	bool render(bool& showWindow);

	void renderOverview();
	void renderSettings();

private:

	// Hotseat
	int _playTime = 0;
	int _resetTime = 0;
	int _minResetTime = 0;
	bool _multiBonus = true;

	// Dependency injection
	Hosting& _hosting;

};