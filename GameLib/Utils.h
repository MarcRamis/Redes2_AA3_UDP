#pragma once

#include <algorithm>

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
