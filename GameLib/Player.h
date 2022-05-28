#pragma once

#include "SFML_Draw.h"
class Player
{
	SFML_Draw* draw;
	float velocity;
	sf::Vector2f direction;

	bool shootKeyPressed = false;

public:
	Player();
	Player(float _posX, float _posY, float _velocity);

	void Update();
	void GetInputs();

	sf::Vector2f GetPlayerPos();
};

