#include <iostream>
#include <SFML/Network.hpp>

#include "Game.h"

int main()
{
	unsigned int randomSeed = unsigned int(time(NULL));
	srand(randomSeed);

	Game* game = new Game();

	std::string type;
	while (type != "e")
	{
		std::cin >> type;
		std::cout << std::endl;
	}

	return 0;
}