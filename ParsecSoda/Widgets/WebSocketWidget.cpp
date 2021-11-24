#include "WebSocketWidget.h"

WebSocketWidget::WebSocketWidget(Hosting& hosting)
    : _hosting(hosting)
{
}

bool WebSocketWidget::render()
{
    
    WebSocket& _ws = _hosting.getWebSocket();

    // static float indentSize = 0;
    // static ImVec2 dummySize = ImVec2(0.0f, 15.0f);
    // static ImVec2 cursor;
    AppStyle::pushTitle();
    ImGui::SetNextWindowSizeConstraints(ImVec2(100, 50), ImVec2(500, 600));
    ImGui::Begin("WebSocket");
    AppStyle::pushLabel();

    // static ImVec2 size;
    // size = ImGui::GetContentRegionAvail();
    // ImGui::SetNextItemWidth(size.x);

    //ImGui::InputText(" ", _ws_uri, 50, ImGuiInputTextFlags_EnterReturnsTrue);
    ImGui::InputText(" ", _ws_uri, 50);

    if (_ws.connected())
    {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.75f, 0.16f, 0.28f, 1.00f));
        if (ImGui::Button("Disconnect"))
        {
            _hosting.webSocketStop();
        }
    }
    else
    {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.00f, 0.47f, 0.80f, 1.00f));
        if (ImGui::Button("Connect"))
        {
            string conUri(_ws_uri);
            _hosting.webSocketStart(conUri);
        }
    }
    //ImGui::Checkbox("Enable WebSockets", &_websocketsEnabled);


    ImGui::PopStyleColor();
    //ImGui::Text("Adapters");


    AppStyle::pop();
    ImGui::End();
    AppStyle::pop();

    return true;
}