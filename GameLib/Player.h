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
	std::vector<Command*> commands_no_validated;

	Player();
	Player(float _posX, float _posY, float _velocity);

	void Update();
	void GetInputs();

	sf::Vector2f GetPlayerPos();
};

