#pragma once

#include "Player.h"

class Game
{

public:
	
	sf::RenderWindow *windowGame;
	std::vector<Player*> players;
	int maxPlayers = MAX_GAME_PLAYERS;

	Game();
	~Game();
	
	void GenPlayers(int _port);
};