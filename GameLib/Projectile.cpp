#include "Projectile.h"

Projectile::Projectile()
{
	position = sf::Vector2f(0.0f, 0.0f);
	direction = sf::Vector2f(1.0f, 0.0f);
	velocity = 0.1f;
	itemDelete = false;
}

Projectile::Projectile(sf::Vector2f _position, sf::Vector2f _direction, float _velocity, sf::RectangleShape _texture) : position(_position), direction(_direction), velocity(_velocity), texture(_texture)
{
	itemDelete = false;
}

Projectile::~Projectile()
{
}

void Projectile::Update()
{
	texture.move(direction * velocity);
	if (texture.getPosition().x < 10.0f || texture.getPosition().x > WIN_SIZE_X || texture.getPosition().y < 0.0f || texture.getPosition().y > WIN_SIZE_Y)
	{
		itemDelete = true;
	}
}

sf::RectangleShape Projectile::GetTexture()
{
	return texture;
}

bool Projectile::DeleteItem()
{
	return itemDelete;
}
