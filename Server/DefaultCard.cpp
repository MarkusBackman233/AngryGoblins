#include "DefaultCard.h"

DefaultCard::DefaultCard()
{
}

void DefaultCard::SetId(const std::string& id)
{
	m_id = id;
}

std::string DefaultCard::GetId()
{
	return m_id;
}

void DefaultCard::SetManaCost(uint8_t manaCost)
{
	m_manaCost = manaCost;
}

uint8_t DefaultCard::GetManaCost() const
{
	return m_manaCost;
}
