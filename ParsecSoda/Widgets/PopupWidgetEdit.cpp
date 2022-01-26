#include "PopupWidgetEdit.h"

bool PopupWidgetEdit::render(const char* title, bool& showPopup, std::string &oldReason)
{
    static bool result;
    result = false;
    static char newReason[32] = "";
    static ImVec2 BUTTON_SIZE = ImVec2(64, 64);

    //ImGui::SetNextWindowSize(ImVec2(300, 200));
    ImGui::SetNextWindowSize(ImVec2(300, 180));
    AppStyle::pushTitle();
    if (ImGui::BeginPopupModal(title, nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
    {

        static ImVec2 size;
        size = ImGui::GetContentRegionAvail();

        ImGui::BeginChild(title, ImVec2(10, 10));
        ImGui::EndChild();

        AppStyle::pushInput();
        ImGui::SetNextItemWidth(size.x);
        ImGui::InputText("##Reason", newReason, 32);
        AppStyle::pop();

        ImGui::Dummy(ImVec2(0.0f, 10.0f));

        ImGui::Indent(10);
        if (IconButton::render(AppIcons::no, AppColors::negative, BUTTON_SIZE))
        {
            showPopup = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        ImGui::Indent(165);
        if (IconButton::render(AppIcons::yes, AppColors::positive, BUTTON_SIZE))
        {

            oldReason = newReason;

            result = true;
            showPopup = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
    AppStyle::pop();

    return result;
}
