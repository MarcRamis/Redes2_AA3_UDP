#pragma once

#include "Player.h"

class Game
{

public:
	
	sf::RenderWindow *windowGame;
	std::vector<PlayerTex*> players;
	int maxPlayers = MAX_GAME_PLAYERS;
	float delta;
	
	Game();
	~Game();
	
	void GenPlayers(int _port);
	void AddPlayer(sf::Vector2f pos, int _port);

	void Update();
	
	PlayerTex *FindPlayerByPort(int _port);
	void Simulate(CommandList::EType commandType, int _port);
	bool CheckFinalPosition(sf::Vector2f pos1, sf::Vector2f pos2);
};