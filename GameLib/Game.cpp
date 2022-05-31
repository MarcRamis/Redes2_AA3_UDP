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

void Game::Simulate(CommandList::EType commandType, int _port)
{
	for (PlayerTex *p : players)
	{
		if (p->port == _port)
		{
			switch (commandType)
			{
			case CommandList::EType::MOVE_UP:
				p->tex->move(sf::Vector2f(0,-MAX_VEL_PLAYER * delta));
				break;
			case CommandList::EType::MOVE_RIGHT:
				p->tex->move(sf::Vector2f(MAX_VEL_PLAYER * delta, 0));
				break;
			case CommandList::EType::MOVE_DOWN:
				p->tex->move(sf::Vector2f(0, MAX_VEL_PLAYER * delta));
				break;
			case CommandList::EType::MOVE_LEFT:
				p->tex->move(sf::Vector2f(-MAX_VEL_PLAYER * delta, 0));
				break;
			case CommandList::EType::SHOOT_UP:
				std::cout << p->port << ": Shooting up" << std::endl;
				break;
			case CommandList::EType::SHOOT_RIGHT:
				std::cout << p->port << ": Shooting right" << std::endl;
				break;
			case CommandList::EType::SHOOT_DOWN:
				std::cout << p->port << ": Shooting down" << std::endl;
				break;
			case CommandList::EType::SHOOT_LEFT:
				std::cout << p->port << ": Shooting left" << std::endl;
				break;
			default:
				break;
			}
		}
	}
}

bool Game::CheckFinalPosition(sf::Vector2f pos1, sf::Vector2f pos2)
{
	return pos1 == pos2;
}
