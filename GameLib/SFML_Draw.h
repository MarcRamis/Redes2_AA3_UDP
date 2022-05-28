#pragma once
#include <SFML/Graphics.hpp>
#include <thread>
#include <vector>
#include "Constants.h"
#include "Projectile.h"

class SFML_Draw
{
	sf::RenderWindow* window;

	sf::RectangleShape myPlayerTex;

	std::vector<sf::RectangleShape> squares;
	std::vector<Projectile> projectiles;
	
	sf::Vector2f direction;
	float velocity;
	float delta;

public:
	SFML_Draw();
	SFML_Draw(sf::Vector2f _playerPosition, float _velocity);
	~SFML_Draw();

	void UpdateWindow();
	void DrawSquares();
	void DrawPlayer();

	void AddSquare(float _sizeX, float _sizeY);
	void MovePlayer(sf::Vector2f newPos);
	sf::Vector2f GetSquarepos(int _squareID);
	void SetSquarePos(int _squareID, float _newPosX, float _newPosY);
	void DeleteSquare(int _squareID);

	void AddProjectile(float _sizeX, float _sizeY, float _positionX, float _positionY, float _directionX, float _directionY, float _delta);
	void MoveProjectiles();
	void DrawProjectiles();

	sf::RenderWindow* GetWindow();
	float GetDelta();
	void SetDelta(float _delta);

	sf::RectangleShape GetPlayerTex();
};

