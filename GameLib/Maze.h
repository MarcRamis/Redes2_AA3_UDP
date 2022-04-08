#pragma once
#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <random>

#include "Card.h"
#include "Constants.h"

class Organ;
class Medicine;
class Virus;
class Treatment;

class Player;

class Maze
{
public:
	std::stack<Card*> deck;
	std::stack<Card*> discardDeck;

	Maze();
	std::vector<Card*> DealCards(unsigned int maxCardsToDeal);
	void FillDeck(std::vector<Card*> deck);
	void DiscardCard(Player& p, Card* c, int id);
	void DiscardOtherCard(Player& p, Card* c, int i, int id);
	void DiscardAllOtherHandCards(Player& p);
	void DiscardAllOtherHandCards(Player& p, int playerThatUsedCard);
	void DiscardAllHandCards(Player &p);
};