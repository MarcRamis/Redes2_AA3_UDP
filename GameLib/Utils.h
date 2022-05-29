#pragma once

#include <iostream>
#include <algorithm>
#include <future>
#include <string>

static std::string GetLineFromCin() {

	std::string line;
	std::getline(std::cin, line);
	return line;
}

static int GetLineFromCinInt() {

	int line;
	std::cin >> line;
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

//std::queue<int> convert(std::queue<Command::EType> in) {
//	
//	std::queue<int> out;
//	while (!in.empty())
//	{
//		out.push(static_cast<int>(in.front()));
//		in.pop();
//	}
//	return out;
//}

static std::string GenerateName()
{
	std::string names[] = { "Bob", "Marc", "Joel", "Alex", "Eric", "Jose" , "Paul", "Pere"};

	int i;
	i = rand() % 8;

	return names[i];
}

static bool PlayerNamesMatch(std::string _name1, std::string _name2)
{
	char letter1 = _name1.front();
	char letter2 = _name2.front();

	return (letter1 > letter2 - 10 && letter1 < letter2 + 10);
}
