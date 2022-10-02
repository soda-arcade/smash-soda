#include "GuestListWidget.h"

GuestListWidget::GuestListWidget(Hosting& hosting)
    : _hosting(hosting), _guests(hosting.getGuestList()), _modList(_hosting.getModList()),
    _leaderboard(_hosting.getLeaderboardList()), _banList(_hosting.getBanList()), _guestHistory(_hosting.getGuestHistory())
{
}

bool GuestListWidget::render()
{
    AppStyle::pushTitle();
    ImGui::SetNextWindowSizeConstraints(ImVec2(150, 150), ImVec2(800, 900));
    ImGui::Begin("Guests", (bool*)0);
    AppStyle::pushInput();

    ImVec2 size = ImGui::GetContentRegionAvail();

    ImGui::BeginChild("Guest List", ImVec2(size.x, size.y));

    AppStyle::pushLabel();
    ImGui::Text("FILTER GUESTS");
    AppStyle::pop();
    ImGui::SetNextItemWidth(size.x-42);
    ImGui::InputText("### Guest List Filter", _filterText, 256);
    TitleTooltipWidget::render("Filter Guests", "Type user Name or ID to filter.");

    ImGui::SameLine();
    ImGui::Checkbox("##Blank bans", &_filterNoReason);
    TitleTooltipWidget::render("Filter Guests", "Show only bans with no reason");

    ImGui::Dummy(ImVec2(0, 5));
    //ImGui::Separator();
    //ImGui::Dummy(ImVec2(0, 5));

    if (ImGui::BeginTabBar("Guest Tabs", ImGuiTabBarFlags_None))
    {
        AppFonts::pushInput();
        AppColors::pushTitle();
        if (ImGui::BeginTabItem("Online"))
        {
            renderOnlineGuests();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Mods"))
        {
            renderModGuests();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Blocked"))
        {
            renderBannedGuests();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("History"))
        {
            renderHistoryGuests();
            ImGui::EndTabItem();
        }

        /* 
        if (ImGui::BeginTabItem("Leaderboard")) {
            renderLeaderboardGuests();
            ImGui::EndTabItem();
        }
        */
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

void GuestListWidget::renderOnlineGuests()
{
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(1, 1));
    ImGui::BeginChild("onlinelist");

    static ImVec2 size;
    size = ImGui::GetContentRegionAvail();
    static size_t popupIndex;
    static string name;
    static uint32_t userID;
    static MyMetrics m;
    static ImVec2 cursor;

    // Guests
    static bool showKickPopup = false;
    static string kickPopupTitle = "";
    static bool showBanPopup = false;
    static string banPopupTitle = "";
    static string filterTextStr;
    static bool filterSuccess = false;
    for (size_t i = 0; i < _guests.size(); ++i)
    {
        name = _guests[i].name;
        userID = _guests[i].userID;
        m = _hosting.getMetrics(_guests[i].id);

        filterTextStr = _filterText;
        if (!filterTextStr.empty())
        {
            filterSuccess = (Stringer::fuzzyDistance(_filterText, name) == 0);
            if (!filterSuccess)
            {
                filterSuccess = (Stringer::fuzzyDistance(_filterText, to_string(userID)) == 0);
            }

            if (!filterSuccess)
            {
                continue;
            }
        }

        IconButton::render(AppIcons::kick, AppColors::primary, ImVec2(24, 24));
        if (ImGui::IsItemActive())
        {
            showKickPopup = true;
            kickPopupTitle = string("Kick##Popup");
            popupIndex = i;
            ImGui::OpenPopup(kickPopupTitle.c_str());
        }
        TitleTooltipWidget::render(
            "Kick user",
            (string("Press to kick ") + name + "").c_str()
        );

        ImGui::SameLine();

        IconButton::render(AppIcons::block, AppColors::primary, ImVec2(24, 24));
        if (ImGui::IsItemActive())
        {
            showBanPopup = true;
            banPopupTitle = string("Ban##Popup");
            popupIndex = i;
            ImGui::OpenPopup(banPopupTitle.c_str());
        }
        TitleTooltipWidget::render(
            "Ban user",
            (string("Press to ban ") + name + "").c_str()
        );

        if (i == popupIndex)
        {
            if (ConfirmPopupWidget::render(
                kickPopupTitle.c_str(),
                showKickPopup,
                ("Kick\n#" + to_string(userID) + "\n" + name).c_str()
            ))
            {
                _hosting.sendHostMessage((
                    string("!kick ") + to_string(userID)
                ).c_str(), true);
            }

            if (ConfirmPopupWidget::render(
                banPopupTitle.c_str(),
                showBanPopup,
                ("Ban\n#" + to_string(userID) + "\n" + name).c_str()
            ))
            {
                _hosting.sendHostMessage((
                    string("!ban ") + to_string(userID)
                ).c_str(), true);
            }
        }

        ImGui::SameLine();

        cursor = ImGui::GetCursorPos();
        ImGui::BeginGroup();

        AppStyle::pushInput();
        ImGui::Text("%s", name.c_str());
        
        ImGui::SameLine();
        ImGui::Indent(85);
        ImGui::Text("#%d", userID);
        AppStyle::pop();
        ImGui::Unindent(85);
        AppFonts::pushInput();
        if (m.congested == 2)
        {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.75f, 0.16f, 0.28f, 1.00f));
        }
        else if (m.congested == 1)
        {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.75f, 0.75f, 0.0f, 1.00f));
        }
        else
        {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.00f, 0.67f, 0.41f, 1.00f));
        }
        if (m.metrics.packetsSent < 1)
        {
            ImGui::Text("-");
        }
        else
        {
            ImGui::Text("%.0fms  B:%.1f  D:%u/%.1f  E:%.1f  N:%u/%u",
                m.metrics.networkLatency,
                m.metrics.bitrate,
                m.metrics.queuedFrames,
                m.metrics.decodeLatency,
                m.metrics.encodeLatency,
                m.metrics.slowRTs,
                m.metrics.fastRTs
            );
        }
        AppStyle::pop();
        ImGui::Dummy(ImVec2(0.0f, 5.0f));
        ImGui::EndGroup();
        ImGui::SetCursorPos(cursor);
        ImGui::Button((string("##") + to_string(i + 1)).c_str(), ImVec2(size.x - 45, 40));

        if (ImGui::BeginDragDropSource())
        {
            ImGui::SetDragDropPayload("Guest", &i, sizeof(int));

            AppFonts::pushInput();
            AppColors::pushPrimary();
            ImGui::Text("%s", name.c_str());
            AppColors::pop();
            AppFonts::pop();

            AppStyle::pushLabel();
            ImGui::Text("Drop into a Gamepad to assign.");
            AppStyle::pop();

            ImGui::EndDragDropSource();
        }
    }
    
    ImGui::EndChild();
    ImGui::PopStyleVar();
}

void GuestListWidget::renderModGuests() {
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(1, 1));
    ImGui::BeginChild("moddedlist");

    static bool showEditPopup = false;
    static size_t popupEditIndex;
    static bool showUnmodPopup = false;
    static string popupTitle = "";
    static size_t popupIndex;
    static string name;
    static uint32_t userID;
    static vector<GuestData>& _moddedGuests = _modList.getGuests();
    static string filterTextStr;
    static bool filterSuccess = false;

    for (size_t i = 0; i < _moddedGuests.size(); ++i)
    {
        name = _moddedGuests[i].name;
        userID = _moddedGuests[i].userID;

        filterTextStr = _filterText;
        if (!filterTextStr.empty())
        {
            filterSuccess = (Stringer::fuzzyDistance(_filterText, name) == 0);
            if (!filterSuccess)
            {
                filterSuccess = (Stringer::fuzzyDistance(_filterText, to_string(userID)) == 0);
            }

            if (!filterSuccess)
            {
                continue;
            }
        }

        IconButton::render(AppIcons::userOff, AppColors::primary, ImVec2(30, 30));
        if (ImGui::IsItemActive())
        {
            popupTitle = string("Unmod##Popup");
            showUnmodPopup = true;
            popupIndex = i;
            ImGui::OpenPopup(popupTitle.c_str());
        }
        TitleTooltipWidget::render(
            "Mod user",
            (string("Press to unmod ") + name + "").c_str()
        );

        if (i == popupIndex)
        {
            if (ConfirmPopupWidget::render(
                popupTitle.c_str(),
                showUnmodPopup,
                ("Unmod\n#" + to_string(userID) + "\n" + name).c_str()
            ))
            {
                _hosting.sendHostMessage((
                    string("!unmod ") + to_string(userID)
                    ).c_str(), true);
            }
        }

        ImGui::SameLine();

        ImGui::BeginGroup();
        AppStyle::pushInput();
        ImGui::Text("#%d", userID);
        ImGui::SameLine();
        ImGui::Indent(85);
        ImGui::Text("%s", name.c_str());
        AppStyle::pop();
        ImGui::Unindent(85);
        AppFonts::pushInput();
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.40f, 0.50f, 0.60f, 1.00f));
        //ImGui::Text(reason.c_str());
        AppStyle::pop();
        ImGui::Dummy(ImVec2(0.0f, 5.0f));
        ImGui::EndGroup();
    }

    ImGui::EndChild();
    ImGui::PopStyleVar();
}

void GuestListWidget::renderBannedGuests()
{
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(1, 1));
    ImGui::BeginChild("bannedlist");

    static bool showEditPopup = false;
    static size_t popupEditIndex;
    static string reason;
    static bool showUnbanPopup = false;
    static string popupTitle = "";
    static size_t popupIndex;
    static string name;
    static uint32_t userID;
    static vector<GuestData>& _bannedGuests = _banList.getGuests();
    static string filterTextStr;
    static bool filterSuccess = false;

    for (size_t i = 0; i < _bannedGuests.size(); ++i)
    {
        name = _bannedGuests[i].name;
        userID = _bannedGuests[i].userID;
        reason = _bannedGuests[i].reason;

        filterTextStr = _filterText;
        if (!filterTextStr.empty())
        {
            filterSuccess = (Stringer::fuzzyDistance(_filterText, name) == 0);
            if (!filterSuccess)
            {
                filterSuccess = (Stringer::fuzzyDistance(_filterText, to_string(userID)) == 0);
            }
            if (!filterSuccess && reason.size() > 0)
            {
                filterSuccess = (Stringer::fuzzyDistance(_filterText, reason) == 0);
            }

            if (!filterSuccess)
            {
                continue;
            }
        }

        if (_filterNoReason && reason.size() > 0) continue;

        IconButton::render(AppIcons::userOff, AppColors::primary, ImVec2(30, 30));
        if (ImGui::IsItemActive())
        {
            popupTitle = string("Unban##Popup");
            showUnbanPopup = true;
            popupIndex = i;
            ImGui::OpenPopup(popupTitle.c_str());
        }
        TitleTooltipWidget::render(
            "Blocked user",
            (string("Press to unban ") + name + "").c_str()
        );

        if (i == popupIndex)
        {
            if (ConfirmPopupWidget::render(
                popupTitle.c_str(),
                showUnbanPopup,
                ("Unban\n#" + to_string(userID) + "\n"+ name).c_str()
            ))
            {
                _hosting.sendHostMessage((
                    string("!unban ") + to_string(userID)
                ).c_str(), true);
            }
        }

        ImGui::SameLine();
        IconButton::render(AppIcons::editReason, AppColors::primary, ImVec2(30, 30));
        if (ImGui::IsItemActive())
        {
            showEditPopup = true;
            popupEditIndex = i;
            ImGui::OpenPopup(string("Add reason").c_str());
        }
        TitleTooltipWidget::render("Add reason", string("Press to add reason").c_str());
        if (i == popupEditIndex)
        {
            if (PopupWidgetEdit::render(string("Add reason").c_str(), showEditPopup, reason))
            {
                _bannedGuests[i].reason = reason;
                 MetadataCache::saveBannedUsers(_bannedGuests);
            }
        }

        ImGui::SameLine();
        
        ImGui::BeginGroup();
        AppStyle::pushInput();
        ImGui::Text("#%d", userID);
        ImGui::SameLine();
        ImGui::Indent(85);
        ImGui::Text("%s", name.c_str());
        AppStyle::pop();
        ImGui::Unindent(85);
        AppFonts::pushInput();
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.40f, 0.50f, 0.60f, 1.00f));
        ImGui::Text(reason.c_str());
        AppStyle::pop();
        ImGui::Dummy(ImVec2(0.0f, 5.0f));
        ImGui::EndGroup();
    }

    ImGui::EndChild();
    ImGui::PopStyleVar();
}


void GuestListWidget::renderHistoryGuests()
{
    ImGui::BeginChild("historylist");
    static bool showBanPopup = false;
    static string popupTitle = "";
    static size_t popupIndex;
    static string name;
    static uint32_t userID;
    static string filterTextStr;
    static bool filterSuccess = false;

    for (size_t i = 0; i < _guestHistory.size(); ++i)
    {
        name = _guestHistory[i].name;
        userID = _guestHistory[i].userID;

        filterTextStr = _filterText;
        if (!filterTextStr.empty())
        {
            filterSuccess = (Stringer::fuzzyDistance(_filterText, name) == 0);
            if (!filterSuccess)
            {
                filterSuccess = (Stringer::fuzzyDistance(_filterText, to_string(userID)) == 0);
            }

            if (!filterSuccess)
            {
                continue;
            }
        }

        IconButton::render(AppIcons::block, AppColors::primary, ImVec2(30, 30));
        if(ImGui::IsItemActive())
        {
            showBanPopup = true;
            popupTitle = string("Ban##Popup");
            popupIndex = i;
            ImGui::OpenPopup(popupTitle.c_str());
        }

        TitleTooltipWidget::render("Allowed user", (string("Press to ban ") + name).c_str());

        if (i == popupIndex)
        {
            if (ConfirmPopupWidget::render(
                popupTitle.c_str(),
                showBanPopup,
                ("Ban\n#" + to_string(userID) + "\n" + name).c_str()
            ))
            {
                _hosting.sendHostMessage((
                    string("!ban ") + to_string(userID)
                ).c_str(), true);
            }
        }

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(1, 1));
        ImGui::SameLine();

        ImGui::BeginGroup();
        AppStyle::pushLabel();
        ImGui::TextWrapped("(# %d)\t", userID);
        AppStyle::pop();
        AppStyle::pushInput();
        ImGui::TextWrapped(name.c_str());
        AppStyle::pop();
        ImGui::Dummy(ImVec2(0.0f, 5.0f));
        ImGui::EndGroup();
        ImGui::PopStyleVar();
    }
    ImGui::EndChild();
}

void GuestListWidget::renderLeaderboardGuests() {
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(1, 1));
    ImGui::BeginChild("leaderboardlist");

    static bool showEditPopup = false;
    static size_t popupEditIndex;
    static bool showLeaderboardPopup = false;
    static string popupTitle = "";
    static size_t popupIndex;
    static string name;
    static uint32_t userID;
    static vector<GuestData>& _leaderboardGuests = _leaderboard.getGuests();
    static string filterTextStr;
    static bool filterSuccess = false;

    for (size_t i = 0; i < _leaderboardGuests.size(); ++i)
    {
        name = _leaderboardGuests[i].name;
        userID = _leaderboardGuests[i].userID;

        filterTextStr = _filterText;
        if (!filterTextStr.empty())
        {
            filterSuccess = (Stringer::fuzzyDistance(_filterText, name) == 0);
            if (!filterSuccess)
            {
                filterSuccess = (Stringer::fuzzyDistance(_filterText, to_string(userID)) == 0);
            }

            if (!filterSuccess)
            {
                continue;
            }
        }

        IconButton::render(AppIcons::userOff, AppColors::primary, ImVec2(30, 30));
        if (ImGui::IsItemActive())
        {
            popupTitle = string("Remove##Popup");
            showLeaderboardPopup = true;
            popupIndex = i;
            ImGui::OpenPopup(popupTitle.c_str());
        }
        TitleTooltipWidget::render(
            "Leaderboard user",
            (string("Press to remove from leaderboard ") + name + "").c_str()
        );

        if (i == popupIndex)
        {
            /* Remove from leaderboard
            if (ConfirmPopupWidget::render(
                popupTitle.c_str(),
                showLeaderboardPopup,
                ("Remove\n#" + to_string(userID) + "\n" + name).c_str()
            ))
            {
                _hosting.sendHostMessage((
                    string("!unmod ") + to_string(userID)
                    ).c_str(), true);
            }
            */
        }

        ImGui::SameLine();

        ImGui::BeginGroup();
        AppStyle::pushInput();
        ImGui::Text("#%d", userID);
        ImGui::SameLine();
        ImGui::Indent(85);
        ImGui::Text("%s", name.c_str());
        AppStyle::pop();
        ImGui::Unindent(85);
        AppFonts::pushInput();
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.40f, 0.50f, 0.60f, 1.00f));
        //ImGui::Text(reason.c_str());
        AppStyle::pop();
        ImGui::Dummy(ImVec2(0.0f, 5.0f));
        ImGui::EndGroup();
    }

    ImGui::EndChild();
    ImGui::PopStyleVar();
}