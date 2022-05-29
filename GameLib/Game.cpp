#include "Game.h"

Game::Game()
{
	std::thread tNewGame(&Game::Update, this);
	tNewGame.detach();
}

Game::~Game()
{

}

void Game::GenPlayers(int _port)
{
	switch (players.size())
	{
	case 0:
		AddPlayer(sf::Vector2f(200, 200), _port);

		break;
	case 1:
		AddPlayer(sf::Vector2f(400, 400), _port);

		break;
	case 2:
		AddPlayer(sf::Vector2f(450, 150), _port);

		break;
	case 3:
		AddPlayer(sf::Vector2f(500, 500), _port);
		break;
	case 4:
		AddPlayer(sf::Vector2f(300, 300), _port);
		break;
	}
}

void Game::AddPlayer(sf::Vector2f pos, int _port)
{
	PlayerTex* p = new PlayerTex(new sf::RectangleShape(sf::Vector2f(50, 50)), _port);
	p->tex->setPosition(pos.x, pos.y);
	players.push_back(p);
}

void Game::Update()
{
	windowGame = new sf::RenderWindow(sf::VideoMode(WIN_SIZE_X, WIN_SIZE_Y), "My window");
	sf::Clock clock;

	while (windowGame->isOpen())
	{
		delta = clock.restart().asSeconds();
		
		//// CHECK EVENTS
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (windowGame->pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				windowGame->close();
		}

		////////// DRAW
		// clear the window with black color
		windowGame->clear(sf::Color::Black);
		
		for (int i = 0; i < players.size(); i++)
		{
			windowGame->draw(*players.at(i)->tex);
		}

		// end the current frame
		windowGame->display();
	}
}

PlayerTex *Game::FindPlayerByPort(int _port)
{
	for (PlayerTex* p : players)
	{
		if (_port == p->port) return p;
	}

	return nullptr;
}
