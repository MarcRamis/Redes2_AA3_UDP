#pragma once

#include <algorithm>
#include <future>
#include <string>

#include "Command.h"

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

std::vector<Command::EType> convert(std::vector<int> const& in) {
	std::vector<Command::EType> out;
	out.reserve(in.size());
	
	std::transform(in.begin(), in.end(), std::back_inserter(out),
		[](int n) { return static_cast<Command::EType>(n); });
	return out;
}