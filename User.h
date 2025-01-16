#pragma once
#include <string>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <rapidjson/document.h>
#include <rapidjson/reader.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <vector>


namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

class User
{
public:
	User(websocket::stream<tcp::socket>* socket);

	void SetUsername(const std::string& username);


	unsigned long long GetId() const;
	std::string GetUsername();
#undef SendMessage


	void SendMessage(const rapidjson::Document& messageContents);

	const std::vector<std::string>& GetMessageBuffer();

private:

	std::vector<std::string> m_messageBuffer;
	websocket::stream<tcp::socket>* m_socket;
	unsigned long long m_userId;
	std::string m_username;
};
