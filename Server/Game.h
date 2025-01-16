#pragma once
#include <vector>

class DefaultCardManager;
class User;

class Game
{
public:
	Game(User* firstUser, User* secondUser, DefaultCardManager* defaultCardManager);
	~Game();

private:

	std::vector<User*> m_usersInGame;

	DefaultCardManager* m_defaultCardManager;

};

