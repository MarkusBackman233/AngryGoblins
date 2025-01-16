#include "ReceiveClientMessage.h"
#include <iostream>
#include <string>
#include "User.h"
#include "UserManager.h"
#include "SendClientMessage.h"
#define AddMessageFunction(name, function) m_messageFunctions.emplace(name, std::bind(&ReceiveClientMessage::function, this, std::placeholders::_1, std::placeholders::_2))


void ReceiveClientMessage::SetupMessageFunctionMap()
{
    AddMessageFunction("CardPosition", SetCardPosition);
    AddMessageFunction("LoginRequest", LoginRequest);
    AddMessageFunction("FindGame", FindGame);
}

ReceiveClientMessage::ReceiveClientMessage(GameQueue* gameQueue) 
    : m_gameQueue(gameQueue)
{
    SetupMessageFunctionMap();
}

void ReceiveClientMessage::Handle(const std::string& jsonString, User* user)
{
    rapidjson::Document document;
    document.Parse(jsonString.c_str());
    if (document.HasParseError()) 
    {
        std::cerr << "JSON parsing error: " << document.GetParseError() << std::endl;
        return;
    }
    if (!document.IsObject()) 
    {
        std::cerr << "JSON is not an object" << std::endl;
        return;
    }
    if (document.HasMember("type") && document["type"].IsString()) 
    {
        std::string type = document["type"].GetString();
        if (document.HasMember("content")) 
        {
            auto messageFunctionIt = m_messageFunctions.find(type);
            if (messageFunctionIt != m_messageFunctions.end())
            {
                messageFunctionIt->second(document["content"], user);
            }
            else
            {
                std::cerr << "Could not find message function string" << std::endl;
            }
        }
        else
        {
            std::cerr << "JSON message has no content" << std::endl;
        }
    }
    else
    {
        std::cerr << "JSON message has no type" << std::endl;
    }
}


void ReceiveClientMessage::SetCardPosition(const rapidjson::Value& messageContents, User* user)
{

    if (messageContents.HasMember("cardId"))
    {
        std::cout << "Card id: " << messageContents["cardId"].GetInt() << std::endl;
    }
    if (messageContents.HasMember("position"))
    {
        std::cout <<"Placement id: " << messageContents["position"].GetInt() << std::endl;
    }
    
}

void ReceiveClientMessage::LoginRequest(const rapidjson::Value& messageContents, User* user)
{
    if (messageContents.HasMember("username"))
    {
        std::cout << "user: " << messageContents["username"].GetString()  << " has logged in!" << std::endl;
        user->SetUsername(messageContents["username"].GetString());
    }

    UserManager::AddUser(user);
}

void ReceiveClientMessage::FindGame(const rapidjson::Value& messageContents, User* user)
{
    m_gameQueue->RegisterUserLookingForGame(user);
}
