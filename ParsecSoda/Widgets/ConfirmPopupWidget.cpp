#include "ConfirmPopupWidget.h"

bool ConfirmPopupWidget::render(const char* title, bool& showPopup, std::string textString)
{
    static bool result;
    result = false;
    static ImVec2 BUTTON_SIZE = ImVec2(64, 64);
    static ImVec2 res;
    static ImVec2 cursor;
    res = ImGui::GetMainViewport()->Size;
    ImVec2 windowSize(300, 130);
    if (textString.size() > 0) windowSize.y += 80;
    // open window in center of window
    //ImGui::SetNextWindowPos(ImVec2((res.x/2)-(windowSize.x/2), (res.y/2)-(windowSize.y/2)));
    // open window next to mouse cursor
    static ImVec2 popupPosition;
    popupPosition = ImGui::GetCursorScreenPos();
    if (popupPosition.x < 0) popupPosition.x = 5;
    else if (popupPosition.x > (res.x - windowSize.x)) popupPosition.x = res.x - windowSize.x - 5;
    if (popupPosition.y < 0) popupPosition.y = 5;
    else if (popupPosition.y > (res.y - windowSize.y)) popupPosition.y = res.y - windowSize.y - 5;
    ImGui::SetNextWindowPos(popupPosition);
    ImGui::SetNextWindowSize(windowSize);

    AppStyle::pushTitle();
    if (ImGui::BeginPopupModal(title, nullptr, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
    {
        ImVec2 size = ImGui::GetContentRegionAvail();

        if (textString.size() > 0)
        {
            cursor = ImGui::GetCursorPos();
            ImGui::SetCursorPos(ImVec2(cursor.x + 10, cursor.y + 10));
            AppStyle::pushInput();
            ImGui::Text(textString.c_str());
            AppStyle::pop();
        }

        ImGui::SetCursorPos(ImVec2(25, size.y - 50));
        if (IconButton::render(AppIcons::no, AppColors::negative, BUTTON_SIZE))
        {
            showPopup = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(size.x - 77, size.y - 50));
        if (IconButton::render(AppIcons::yes, AppColors::positive, BUTTON_SIZE))
        {
            result = true;
            showPopup = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
    AppStyle::pop();

    return result;
}
