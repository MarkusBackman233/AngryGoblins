#pragma once
#include <string>

enum CardType
{
	Character,
	Spell
};

class DefaultCard
{
public:
	DefaultCard();


	void SetId(const std::string& id);
	std::string GetId();

	void SetManaCost(uint8_t manaCost);
	uint8_t GetManaCost() const;

private:
	std::string m_id;
	uint8_t m_manaCost;
};

