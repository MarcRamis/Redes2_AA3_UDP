#pragma once

#include "Player.h"

struct PlayerTex
{
	sf::RectangleShape* tex;
	int port;
	
	PlayerTex() {};
	PlayerTex(sf::RectangleShape* _tex, int _port) : tex(_tex), port(_port){};
};

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
};