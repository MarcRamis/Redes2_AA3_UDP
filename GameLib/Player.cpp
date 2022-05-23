#include "Player.h"
#include <iostream>

Player::Player()
{
	draw = new SFML_Draw();
	draw->AddSquare(150, 150);

	velocity = 0.001f;

	std::thread tInputs(&Player::GetInputs, this);
	tInputs.detach();

}

Player::Player(float _posX, float _posY, float _velocity) : velocity(_velocity)
{
	draw = new SFML_Draw(sf::Vector2f(250.0f, 200.0f), velocity);

	//std::thread tInputs(&Player::GetInputs, this);
	//tInputs.detach();
}

void Player::Update()
{
	
	//draw->MovePlayer();
	//draw->MoveProjectiles();
	draw->UpdateWindow();
}

void Player::GetInputs()
{
	while (true)
	{
		
	}
	
}

sf::Vector2f Player::GetPlayerPos()
{
	return draw->GetSquarepos(0);
}
