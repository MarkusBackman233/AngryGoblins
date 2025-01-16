#include "DefaultMonsterCard.h"

DefaultMonsterCard::DefaultMonsterCard()
{
}

void DefaultMonsterCard::SetAttackDamage(int attackDamage)
{
	m_attackDamage = attackDamage;
}

int DefaultMonsterCard::GetAttackDamage() const
{
	return m_attackDamage;
}

void DefaultMonsterCard::SetHealth(int health)
{
	m_health = health;
}

int DefaultMonsterCard::GetHealth() const
{
	return m_health;
}
