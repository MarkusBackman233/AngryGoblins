#pragma once
#include "DefaultCard.h"
class DefaultMonsterCard :
    public DefaultCard
{
public:
    DefaultMonsterCard();

    void SetAttackDamage(int attackDamage);
    int GetAttackDamage() const;
    void SetHealth(int health);
    int GetHealth() const;

private:
    int m_attackDamage;
    int m_health;
};

