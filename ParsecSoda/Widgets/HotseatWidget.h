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
	bool render();

	void renderOverview();
	void renderSettings();

private:

	// Hotseat
	int _playTime = 0;
	int _resetTime = 0;
	int _minResetTime = 0;

	// Dependency injection
	Hosting& _hosting;

};