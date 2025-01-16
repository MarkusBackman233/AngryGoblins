#include "SendClientMessage.h"

rapidjson::Document SendClientMessage::AddCard(const std::string& cardName, bool isThisUsersCard)
{
	rapidjson::Document doc;
	doc.SetObject();
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

	doc.AddMember("type", "AddCard", allocator);
	if (isThisUsersCard)
	{
		doc.AddMember("id", rapidjson::Value().SetString(cardName.c_str(), allocator), allocator);
	}
	doc.AddMember("isThisUsersCard", isThisUsersCard, allocator);
	return doc;
}

rapidjson::Document SendClientMessage::OnGameFound()
{
	rapidjson::Document doc;
	doc.SetObject();
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

	doc.AddMember("type", "OnGameFound", allocator);
	return doc;
}
