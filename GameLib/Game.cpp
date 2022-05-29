#include "Game.h"

Game::Game()
{
}

Game::~Game()
{

}

void Game::GenPlayers(int _port)
{
	switch (players.size())
	{
	case 0:
		
	{
		Player* p = new Player(200, 200);
		p->NewWindow();
		p->port = _port;
		windowGame = p->GetWindow();
		players.push_back(p);
	}

		
		break;
	case 1:
		
	{
		Player* p = new Player(400, 400);
		p->port = _port;
		p->SetWindow(windowGame);
		players.push_back(p);
	}

		
		break;
	case 2:

	{
		Player* p = new Player(450, 150);
		p->port = _port;
		p->SetWindow(windowGame);
		players.push_back(p);
	}

		break;
	case 3:

	{
		Player* p = new Player(500, 500);
		p->port = _port;
		p->SetWindow(windowGame);
		players.push_back(p);
	}


		break;
	case 4:

	{
		Player* p = new Player(300, 300);
		p->port = _port;
		p->SetWindow(windowGame);
		players.push_back(p);
	}

		break;
	}
}
