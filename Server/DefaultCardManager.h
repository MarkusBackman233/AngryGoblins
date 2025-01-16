#pragma once
#include <unordered_map>
#include <string>
#include "DefaultCard.h"


class DefaultCardManager
{
public:
	DefaultCardManager();


private:
	std::unordered_map<std::string,DefaultCard> m_defaultCards;
};

