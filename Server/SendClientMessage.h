#pragma once
#include <rapidjson/document.h>
#include <rapidjson/reader.h>
#include <rapidjson/writer.h>
#include <string>

class SendClientMessage
{
public:
	static rapidjson::Document AddCard(const std::string& cardName, bool isThisUsersCard);
	static rapidjson::Document OnGameFound();
};

