#include "WebSocketWidget.h"

WebSocketWidget::WebSocketWidget(Hosting& hosting)
    : _hosting(hosting)
{
    try
    {
        strcpy_s(_ws_uri, MetadataCache::preferences.websocketURI.c_str());
        strcpy_s(_ws_password, MetadataCache::preferences.websocketPassword.c_str());
    }
    catch (const std::exception&)
    {
        try
        {
            strcpy_s(_ws_uri, "");
            strcpy_s(_ws_password, "");
        }
        catch (const std::exception&) {}
    }
}

bool WebSocketWidget::render()
{
    WebSocket& ws = _hosting.getWebSocket();

    AppStyle::pushTitle();
    ImGui::SetNextWindowSizeConstraints(ImVec2(100, 50), ImVec2(500, 600));
    ImGui::Begin("WebSocket");
    AppStyle::pushLabel();

    static ImVec2 size;
    size = ImGui::GetContentRegionAvail();

    AppStyle::pushInput();
    ImGui::SetNextItemWidth(size.x);
    ImGui::InputText("##uri", _ws_uri, 50);
    ImGui::SetNextItemWidth(size.x);
    ImGui::InputText("##password", _ws_password, 32, ImGuiInputTextFlags_Password);
    AppStyle::pop();

    AppFonts::pushInput();
    string ws_status = ws.status();
    ImGui::Text( ws_status.c_str() );
    AppFonts::pop();

    if (ws_status == "Open")
    {
        AppStyle::pushNegative();
        if (ImGui::Button("Disconnect"))
        {
            _hosting.webSocketStop();
        }
        AppStyle::pop();
    }
    else if (ws_status == "Failed" || ws_status == "Closed")
    {
        AppStyle::pushPositive();
        if (ImGui::Button("Connect"))
        {
            MetadataCache::preferences.websocketURI = _ws_uri;
            MetadataCache::preferences.websocketPassword = _ws_password;
            _hosting.webSocketStart(MetadataCache::preferences.websocketURI, MetadataCache::preferences.websocketPassword);
        }
        AppStyle::pop();
    }

    AppStyle::pop();
    ImGui::End();
    AppStyle::pop();

    return true;
}