#include "../Hosting.h"
extern Hosting g_hosting;
#include "WebSocket.h"

WebSocket WebSocket::instance;

WebSocket::WebSocket() {
    // Constructor implementation (if needed)
}

void WebSocket::createServer(uint16_t port) {
    if (!_asioInited) {
        server_.init_asio();
        _asioInited = true;
    }
    else
    {
        server_.reset();
    }
    server_.set_open_handler(std::bind(&WebSocket::onOpen, this, std::placeholders::_1));
    server_.set_close_handler(std::bind(&WebSocket::onClose, this, std::placeholders::_1));
    server_.set_message_handler(std::bind(&WebSocket::onMessage, this, std::placeholders::_1, std::placeholders::_2));
    
    serverThread_ = std::thread([this, port]() {
        try {
            server_.listen(port);
            server_.start_accept();
            isRunning_ = true;
            g_hosting.logMessage("WebSocket server started.");

            // Start overlay
            if (Config::cfg.overlay.enabled) {
                launchOverlay();
            }

            server_.run();
        } catch (const std::exception& e) {
            g_hosting.logMessage("Server error: " + std::string(e.what()));
        }
        isRunning_ = false;
        g_hosting.logMessage("WebSocket server stopped.");
    });
}

void WebSocket::stopServer() {
    server_.stop_listening();
    server_.stop();
    if (serverThread_.joinable()) {
        serverThread_.join();
    }
    if (Config::cfg.overlay.enabled) {
        closeOverlay();
    }
    isRunning_ = false;
}

void WebSocket::sendMessageToAll(const std::string& message) {
    //g_hosting.logMessage("Sending WebSocket message to all clients.");

    // If not running, return
    if (!isRunning_) {
        return;
    }

    // If no clients are connected, return
    if (clients_.empty()) {
        return;
    }

    //std::lock_guard<std::mutex> lock(clientsMutex_);
    for (auto& client : clients_) {
        server_.send(client, message, websocketpp::frame::opcode::text);
    }
}

bool WebSocket::isRunning() {
    return isRunning_;
}

void WebSocket::onOpen(websocketpp::connection_hdl hdl) {
    std::lock_guard<std::mutex> lock(clientsMutex_);
    clients_.insert(hdl);
    g_hosting.logMessage("WebSocket client connected.");
}

void WebSocket::onClose(websocketpp::connection_hdl hdl) {
    std::lock_guard<std::mutex> lock(clientsMutex_);
    clients_.erase(hdl);
}

void WebSocket::onMessage(websocketpp::connection_hdl hdl, server::message_ptr msg) {
    json j = json::parse(msg->get_payload());
    if (j["event"] == "chat:message") {
        string message;
        j.at("data").get_to(message);

        g_hosting.sendHostMessage(message.c_str());
    } 
    else if (j["event"] == "chat:external") {
        string source;
        string message;
        string user;

        j.at("data").at("user").at("display-name").get_to(user);
        j.at("data").at("message").get_to(message);
        try {
			j.at("data").at("source").get_to(source);
        }
        catch (json::out_of_range& e) {
			source = "Twitch";
		}

        g_hosting.messageFromExternalSource(source, user, message);
    }
}

void WebSocket::launchOverlay() {

    // Get the path of the executable
    wchar_t exePath[MAX_PATH];
    GetModuleFileName(NULL, exePath, MAX_PATH);

    // Extract the directory from the path
    wchar_t* lastBackslash = wcsrchr(exePath, L'\\');
    if (lastBackslash != NULL) {
        *lastBackslash = L'\0'; // Replace the last backslash with null terminator
    }

    // Construct the full path to the overlay executable
    wchar_t overlayPath[MAX_PATH];
    swprintf_s(overlayPath, MAX_PATH, L"%s\\overlay\\smash-soda-overlay.exe", exePath);

    // Create the process
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Start the child process. 
    if (!CreateProcess(overlayPath,	// No module name (use command line)
        (LPWSTR)"",			        // Command line
        NULL,						// Process handle not inheritable
        NULL,						// Thread handle not inheritable
        FALSE,						// Set handle inheritance to FALSE
        0,							// No creation flags
        NULL,						// Use parent's environment block
        NULL,						// Use parent's starting directory 
        &si,						// Pointer to STARTUPINFO structure
        &pi)						// Pointer to PROCESS_INFORMATION structure
        ) {

        return;
    }

    // Save the process ID
    overlayPID = pi.dwProcessId;

}

void WebSocket::closeOverlay() {
    if (overlayPID != 0) {
        HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, overlayPID);
        if (hProcess != NULL) {
            TerminateProcess(hProcess, 0);
            CloseHandle(hProcess);
        }
    }
}