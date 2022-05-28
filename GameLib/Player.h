#pragma once

#include "SFML_Draw.h"
#include "Command.h"

class Player
{
	SFML_Draw* draw;
	float velocity;
	sf::Vector2f direction;

	bool shootKeyPressed = false;

public:
	
	// Commands
	std::queue<Command::EType> tmp_Commands;

	Player();
	Player(float _posX, float _posY, float _velocity);

	void Update();
	void GetInputs();

	void Shoot(float dirX, float dirY);
	void ClearCommands();

	sf::Vector2f GetPlayerPos();
};

