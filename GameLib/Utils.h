#pragma once

#include <algorithm>
#include <future>
#include <string>

static std::string GetLineFromCin() {

	std::cout << "Write a message: | 'e' to close program " << std::endl;
	std::string line;
	std::getline(std::cin, line);
	return line;
}

static unsigned int GenerateSalt()
{
	unsigned int x;
	x = rand() & 0xff;
	x |= (rand() & 0xff) << 8;
	x |= (rand() & 0xff) << 16;
	x |= (rand() & 0xff) << 24;

	return x;
}

static int GenerateChallenge()
{
	unsigned int x;
	x = rand() % 1001;

	return x;
}

static unsigned int CombineSalts(unsigned int saltClient, unsigned int saltServer)
{
	return saltClient && saltServer;
}
