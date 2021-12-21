#pragma once

#define _WINSOCKAPI_
#define ASIO_STANDALONE
#define _WEBSOCKETPP_CPP11_STRICT_
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>

typedef websocketpp::client<websocketpp::config::asio_client> client;
typedef websocketpp::config::asio_client::message_type::ptr message_ptr;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

class connection_metadata {
public:
    typedef websocketpp::lib::shared_ptr<connection_metadata> ptr;
    connection_metadata(int id, websocketpp::connection_hdl hdl, std::string uri);
    void on_open(client* c, websocketpp::connection_hdl hdl);
    void on_fail(client* c, websocketpp::connection_hdl hdl);
    void on_close(client* c, websocketpp::connection_hdl hdl);
    void on_message(websocketpp::connection_hdl, client::message_ptr msg);
    websocketpp::connection_hdl get_hdl() const;
    int get_id() const;
    std::string get_status() const;
    void record_sent_message(std::string message);
    friend std::ostream& operator<< (std::ostream& out, connection_metadata const& data);
private:
    int m_id;
    websocketpp::connection_hdl m_hdl;
    std::string m_status;
    std::string m_uri;
    std::string m_server;
    std::string m_error_reason;
    std::vector<std::string> m_messages;
};

class websocket_endpoint {
public:
    websocket_endpoint();
    ~websocket_endpoint();
    int connect(std::string const& uri);
    void close(int id, websocketpp::close::status::value code, std::string reason);
    void send(int id, std::string message);
    connection_metadata::ptr get_metadata(int id) const;
private:
    typedef std::map<int, connection_metadata::ptr> con_list;
    client m_endpoint;
    websocketpp::lib::shared_ptr<websocketpp::lib::thread> m_thread;
    con_list m_connection_list;
    int m_next_id;
};

class WebSocket {
public:
    WebSocket();
    bool connected();
    void start(string uri);
    void close();
    void handle_message(string msg);
    void sendMetrics(vector<ParsecMetrics> metrics);
    int con_id{ -1 };
    websocket_endpoint endpoint;


};