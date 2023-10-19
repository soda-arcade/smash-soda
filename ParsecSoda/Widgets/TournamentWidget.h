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

class TournamentWidget {
public:

	TournamentWidget(Hosting& hosting);
	bool render();

private:

	// Dependency injection
	Hosting& _hosting;

};