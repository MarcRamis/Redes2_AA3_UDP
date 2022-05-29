#pragma once

#include "SFML_Draw.h"
#include "Command.h"

struct PlayerTex
{
	sf::RectangleShape* tex;
	int port;

	PlayerTex() {};
	PlayerTex(sf::RectangleShape* _tex, int _port) : tex(_tex), port(_port) {};
};

class Player
{
	SFML_Draw* draw = nullptr;
	float velocity;
	sf::Vector2f direction;

	bool shootKeyPressed = false;

public:
	
	// Commands
	std::queue<CommandList::EType> tmp_Commands;
	
	// Other players
	std::vector<PlayerTex*> other_players;

	Player();
	Player(float _posX, float _posY);

	void Update();
	void GetInputs();

	void Shoot(float dirX, float dirY);
	void ClearCommands();

	sf::Vector2f GetPlayerPos();
	void SetPlayerPos(sf::Vector2f newPos);

	void NewWindow();
	sf::RenderWindow* GetWindow();
	void SetWindow(sf::RenderWindow* _window);

	bool IsWindowActive();

	void AddNewPlayer(int posX, int posY, int _port);
};