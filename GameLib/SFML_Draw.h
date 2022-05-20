#pragma once
#include <SFML/Graphics.hpp>
#include <thread>
#include <vector>

class SFML_Draw
{
	sf::RenderWindow* window;

	std::vector<sf::RectangleShape> squares;

public:
	SFML_Draw();
	~SFML_Draw();

	void UpdateWindow();
	void DrawSquares();

	void AddSquare(float _sizeX, float _sizeY);
	void DeleteSquare(int _squareID);

};

