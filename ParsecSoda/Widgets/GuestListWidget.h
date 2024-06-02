#pragma once

#include "../imgui/imgui.h"
#include "../Hosting.h"
#include "../globals/AppIcons.h"
#include "../globals/AppStyle.h"
#include "IconButton.h"
#include "TitleTooltipWidget.h"
#include "ConfirmPopupWidget.h"
#include "PopupWidgetEdit.h"

class GuestListWidget
{
public:
	GuestListWidget(Hosting& hosting);
	bool render();

private:
	void renderOnlineGuests();
	void renderBannedGuests();
	void renderHistoryGuests();
	void renderModdedGuests();
	void renderVIPGuests();

	// Dependency injection
	Hosting& _hosting;
	
	// Attributes
	char _filterText[256] = "";
	bool _filterNoReason = false;
	string _logBuffer;
	vector<Guest>& _guests;
	vector<GuestData>& _guestHistory;
};