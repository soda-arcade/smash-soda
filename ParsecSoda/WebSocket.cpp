#include "Hosting.h"
#include "WebSocket.h"

extern Hosting g_hosting;

connection_metadata::connection_metadata(int id, websocketpp::connection_hdl hdl, std::string uri, std::string password)
    : m_id(id), m_hdl(hdl), m_status("Connecting"), m_uri(uri), m_server("N/A"), m_password(password)
    {}

void connection_metadata::on_open(client* c, websocketpp::connection_hdl hdl) {
    m_status = "Open";
    // Identify
    Guest host = g_hosting.getHost();
    MTY_JSON* jmsg = MTY_JSONObjCreate();
    MTY_JSONObjSetString(jmsg, "type", "identify");
    MTY_JSONObjSetUInt(jmsg, "id", host.id);
    MTY_JSONObjSetUInt(jmsg, "userid", host.userID);
    MTY_JSONObjSetString(jmsg, "username", host.name.c_str());
    MTY_JSONObjSetString(jmsg, "password", m_password.c_str() );
    char* finmsg = MTY_JSONSerialize(jmsg);
    c->send(hdl, finmsg, websocketpp::frame::opcode::text);
}

void connection_metadata::on_fail(client* c, websocketpp::connection_hdl hdl) {
    m_status = "Failed";
}

void connection_metadata::on_close(client* c, websocketpp::connection_hdl hdl) {
    m_status = "Closed";
}

void connection_metadata::on_message(websocketpp::connection_hdl, client::message_ptr msg) {
    if (msg->get_opcode() == websocketpp::frame::opcode::text) {
        //g_hosting.sendHostMessage(msg->get_payload().c_str());
        MTY_JSON *jmsg = MTY_JSONParse(msg->get_payload().c_str());
        if (jmsg)
        {
            uint32_t id;
            uint32_t userid;
            char msg[1023];
            char name[GUEST_NAME_LEN];
            char type;
            MTY_JSONObjGetString(jmsg, "type", &type, 32);
            //if (&type == "chat") {
                MTY_JSONObjGetUInt(jmsg, "id", &id);
                MTY_JSONObjGetUInt(jmsg, "userid", &userid);
                MTY_JSONObjGetString(jmsg, "username", name, GUEST_NAME_LEN);
                MTY_JSONObjGetString(jmsg, "content", msg, 1023);
                Guest guest;
                guest.id = 0;
                guest.userID = userid;
                guest.name = string(name);
                guest.status = Guest::Status::OK;
                g_hosting.handleMessage(msg, guest, false, false, true);
            //}
        }

        //if (!jmsg) {
        //    g_hosting.sendHostMessage(("NO: "+ msg->get_payload()).c_str());
        //}
        //else {
        //    g_hosting.sendHostMessage(("YES: " + msg->get_payload()).c_str());
        //}
    }
    else {
        //m_messages.push_back("<< " + websocketpp::utility::to_hex(msg->get_payload()));
    }
}

websocketpp::connection_hdl connection_metadata::get_hdl() const {
    return m_hdl;
}

int connection_metadata::get_id() const {
    return m_id;
}

std::string connection_metadata::get_status() const {
    return m_status;
}



websocket_endpoint::websocket_endpoint() : m_next_id(0) {
    m_endpoint.clear_access_channels(websocketpp::log::alevel::all);
    m_endpoint.clear_error_channels(websocketpp::log::elevel::all);
    m_endpoint.init_asio();
    m_endpoint.start_perpetual();
    m_thread = websocketpp::lib::make_shared<websocketpp::lib::thread>(&client::run, &m_endpoint);
}

websocket_endpoint::~websocket_endpoint() {
    m_endpoint.stop_perpetual();
    for (con_list::const_iterator it = m_connection_list.begin(); it != m_connection_list.end(); ++it) {
        if (it->second->get_status() != "Open") {
            // Only close open connections
            continue;
        }
        m_endpoint.close(it->second->get_hdl(), websocketpp::close::status::going_away, "");
    }
    m_thread->join();
}

int websocket_endpoint::connect(std::string const& uri, std::string const& password) {
    websocketpp::lib::error_code ec;
    client::connection_ptr con = m_endpoint.get_connection(uri, ec);
    con->add_subprotocol("soda");
    if (ec) return -1;
    int new_id = m_next_id++;
    connection_metadata::ptr metadata_ptr = websocketpp::lib::make_shared<connection_metadata>(new_id, con->get_handle(), uri, password);
    m_connection_list[new_id] = metadata_ptr;
    con->set_open_handler(websocketpp::lib::bind(&connection_metadata::on_open, metadata_ptr, &m_endpoint, websocketpp::lib::placeholders::_1));
    con->set_fail_handler(websocketpp::lib::bind(&connection_metadata::on_fail, metadata_ptr, &m_endpoint, websocketpp::lib::placeholders::_1));
    con->set_close_handler(websocketpp::lib::bind(&connection_metadata::on_close, metadata_ptr, &m_endpoint, websocketpp::lib::placeholders::_1));
    con->set_message_handler(websocketpp::lib::bind(&connection_metadata::on_message, metadata_ptr, websocketpp::lib::placeholders::_1, websocketpp::lib::placeholders::_2));
    m_endpoint.connect(con);
    return new_id;
}

void websocket_endpoint::close(int id, websocketpp::close::status::value code, std::string reason) {
    websocketpp::lib::error_code ec;
    con_list::iterator metadata_it = m_connection_list.find(id);
    if (metadata_it == m_connection_list.end()) return;
    m_endpoint.close(metadata_it->second->get_hdl(), code, reason);
}

void websocket_endpoint::send(int id, std::string message) {
    websocketpp::lib::error_code ec;
    con_list::iterator metadata_it = m_connection_list.find(id);
    if (metadata_it == m_connection_list.end()) return;
    m_endpoint.send(metadata_it->second->get_hdl(), message, websocketpp::frame::opcode::text, ec);
    if (ec) return;
}

connection_metadata::ptr websocket_endpoint::get_metadata(int id) const {
    con_list::const_iterator metadata_it = m_connection_list.find(id);
    if (metadata_it == m_connection_list.end()) {
        return connection_metadata::ptr();
    }
    else {
        return metadata_it->second;
    }
}



WebSocket::WebSocket()
{
    
}

bool WebSocket::connected()
{
    if (con_id != -1)
    {
        connection_metadata::ptr metadata = endpoint.get_metadata(con_id);
        if (metadata->get_status() == "Open")
        {
            return true;
        }
    }
    return false;
}

std::string WebSocket::status() {
    if (con_id != -1)
    {
        connection_metadata::ptr metadata = endpoint.get_metadata(con_id);
        return metadata->get_status();
    }
    return "Closed";
}

void WebSocket::start(string uri, string password)
{
    con_id = endpoint.connect(uri, password);
}

void WebSocket::close()
{
    endpoint.close(con_id, websocketpp::close::status::normal, "");
}

void WebSocket::handle_message(string msg)
{
    endpoint.send(con_id, msg);
}
