#include "DefaultCardManager.h"

#include <rapidjson/document.h>
#include <rapidjson/reader.h>
#include <rapidjson/writer.h>

#include <fstream>  // For file handling
#include <sstream>  // For std::stringstream

#include <filesystem>
#include <Windows.h>

#include "DefaultMonsterCard.h"
#include "DefaultSpellCard.h"

DefaultCardManager::DefaultCardManager()
{
    char path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);




    std::filesystem::path filePath(std::filesystem::current_path() / "HTML\\Cards.json");

    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: ");
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();


    rapidjson::Document document;
    document.Parse(buffer.str().c_str());

    // Check if the JSON has the 'cards' array
    if (document.HasMember("cards") && document["cards"].IsArray()) {
        const rapidjson::Value& cards = document["cards"];

        m_defaultCards.reserve(cards.Size());

        for (rapidjson::SizeType i = 0; i < cards.Size(); ++i) {
            const rapidjson::Value& card = cards[i];

            std::string cardType = card["type"].GetString();
            
            DefaultCard* baseCard = nullptr;

            if (cardType == "Monster")
            {
                DefaultMonsterCard monsterCard;

                monsterCard.SetAttackDamage(card["attack"].GetInt());
                monsterCard.SetHealth(card["health"].GetInt());
                //int cost = card["cost"].GetInt();

                baseCard = dynamic_cast<DefaultCard*>(&monsterCard);
            }
            baseCard->SetId(card["id"].GetString());
            m_defaultCards.emplace(baseCard->GetId(), std::move(*baseCard));
        }
    }
}
