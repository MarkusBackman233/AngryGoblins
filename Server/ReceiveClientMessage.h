#pragma once
#include <rapidjson/document.h>
#include <rapidjson/reader.h>
#include <rapidjson/writer.h>
#include <string>
#include <unordered_map>
#include <functional>

#include "GameQueue.h"

class User;



class ReceiveClientMessage
{
public:
	ReceiveClientMessage(GameQueue* gameQueue);
	void Handle(const std::string& jsonString, User* user);

private:
	void SetupMessageFunctionMap();
	void SetCardPosition(const rapidjson::Value& messageContents, User* user);
	void LoginRequest(const rapidjson::Value& messageContents, User* user);
	void FindGame(const rapidjson::Value& messageContents, User* user);

	typedef std::unordered_map<std::string, std::function<void(const rapidjson::Value& messageContents, User* user)>> MessageFunctionMap;

	MessageFunctionMap m_messageFunctions;

	GameQueue* m_gameQueue;

};

