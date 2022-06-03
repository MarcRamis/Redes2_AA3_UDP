#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"


class Projectile
{
	sf::RectangleShape texture;

	sf::Vector2f position;
	sf::Vector2f direction;
	float velocity;
	bool itemDelete;

public:
	Projectile();
	Projectile(sf::Vector2f _position, sf::Vector2f _direction, float _velocity, sf::RectangleShape _texture);
	~Projectile();

	void Update();
	sf::RectangleShape GetTexture();
	bool DeleteItem();

};

