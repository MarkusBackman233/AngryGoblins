#include "Game.h"
#include "User.h"
#include "SendClientMessage.h"
#include "DefaultCardManager.h"

Game::Game(User* firstUser, User* secondUser, DefaultCardManager* defaultCardManager)
	: m_usersInGame({ firstUser, secondUser })
	, m_defaultCardManager(defaultCardManager)
{
	for (User* user : m_usersInGame)
	{
		user->SendMessage(SendClientMessage::OnGameFound());
		for (User* userNested : m_usersInGame)
		{
			userNested->SendMessage(SendClientMessage::AddCard("angrygoblin", user == userNested));
			userNested->SendMessage(SendClientMessage::AddCard("skeleton", user == userNested));
			userNested->SendMessage(SendClientMessage::AddCard("cultist", user == userNested));
		}
	}
}

Game::~Game()
{

}
