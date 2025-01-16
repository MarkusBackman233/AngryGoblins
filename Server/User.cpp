#include "User.h"
#include <random>

User::User(websocket::stream<tcp::socket>* socket)
	: m_username("Null")
	, m_socket(socket)
{
	std::random_device rd;
	std::mt19937_64 eng(rd());
	std::uniform_int_distribution<unsigned long long> distr;
	m_userId = distr(eng);

}


void User::SetUsername(const std::string& username)
{
	m_username = username;
}

unsigned long long User::GetId() const
{
	return m_userId;
}

std::string User::GetUsername()
{
	return m_username;
}

void User::SendMessage(const rapidjson::Document& messageContents)
{
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	messageContents.Accept(writer);
	
	m_socket->write(net::buffer(std::string(buffer.GetString())));
	// Get the JSON string from the buffer
	//m_messageBuffer.push_back(buffer.GetString());
	//(*m_socket).text((*m_socket).got_text());
	
}

const std::vector<std::string>& User::GetMessageBuffer()
{
	return m_messageBuffer;
}
