#include "Maze.h"

#include "Organ.h"
#include "Virus.h"
#include "Medicine.h"
#include "Treatment.h"

#include "Player.h"

Maze::Maze()
{
	std::vector<Card*> tmpDeck;
	Card *card;

	int counter = 0;
	// CREAR CARTAS DE ÓRGANOS
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			switch (i)
			{
			case 0:
				card = new Organ(Organ::EOrganType::HEART);
				card->id = counter;
				tmpDeck.push_back(card);
				break;
			case 1:
				card = new Organ(Organ::EOrganType::STOMACH);
				card->id = counter;
				tmpDeck.push_back(card);
				break;
			case 2:
				card = new Organ(Organ::EOrganType::BRAIN);
				card->id = counter;
				tmpDeck.push_back(card);
				break;
			case 3:
				card = new Organ(Organ::EOrganType::BONE);
				card->id = counter;
				tmpDeck.push_back(card);
				break;	
			}
			counter++;
		}
	}
	card = new Organ(Organ::EOrganType::JOKER);
	card->id = counter;
	tmpDeck.push_back(card);
	counter++;

	// CREAR CARTAS DE VIRUS
	for (int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			switch (i)
			{
			case 0:
				card = new Virus(Virus::EVirusType::VIRUSHEART);
				card->id = counter;
				tmpDeck.push_back(card);
				break;
			case 1:
				card = new Virus(Virus::EVirusType::VIRUSSTOMACH);
				card->id = counter;
				tmpDeck.push_back(card);
				break;
			case 2:
				card = new Virus(Virus::EVirusType::VIRUSBRAIN);
				card->id = counter;
				tmpDeck.push_back(card);
				break;
			case 3:
				card = new Virus(Virus::EVirusType::VIRUSBONE);
				card->id = counter;
				tmpDeck.push_back(card);
				break;
			}
			counter++;
		}
	}
	card = new Virus(Virus::EVirusType::VIRUSJOKER);
	card->id = counter;
	tmpDeck.push_back(card);
	counter++;
	
	// CREAR CARTAS DE MEDICINAS
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			switch (i)
			{
			case 0:
				card = new Medicine(Medicine::EMedicineType::MEDICINEHEART);
				card->id = counter;
				tmpDeck.push_back(card);
				break;
			case 1:
				card = new Medicine(Medicine::EMedicineType::MEDICINESTOMACH);
				card->id = counter;
				tmpDeck.push_back(card);
				break;
			case 2:
				card = new Medicine(Medicine::EMedicineType::MEDICINEBRAIN);
				card->id = counter;
				tmpDeck.push_back(card);
				break;
			case 3:
				card = new Medicine(Medicine::EMedicineType::MEDICINEBONE);
				card->id = counter;
				tmpDeck.push_back(card);
				break;
			case 4:
				card = new Medicine(Medicine::EMedicineType::MEDICINEJOKER);
				card->id = counter;
				tmpDeck.push_back(card);
				break;
			}
			counter++;
		}
	}

	//CREAR CARTAS DE TRATAMIENTOS
	card = new Treatment(Treatment::ETreatmentType::INFECTION);
	card->id = counter;
	tmpDeck.push_back(card);
	counter++;
	card = new Treatment(Treatment::ETreatmentType::INFECTION);
	card->id = counter;
	tmpDeck.push_back(card);
	counter++;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			switch (i)
			{
			case 0:
				card = new Treatment(Treatment::ETreatmentType::ORGANTHIEF);
				card->id = counter;
				tmpDeck.push_back(card);
				break;
			case 1:
				card = new Treatment(Treatment::ETreatmentType::TRASPLANT);
				card->id = counter;
				tmpDeck.push_back(card);
				break;
			}
			counter++;
		}
	}
	card = new Treatment(Treatment::ETreatmentType::LATEXGLOVE);
	card->id = counter;
	tmpDeck.push_back(card);
	counter++;
	card = new Treatment(Treatment::ETreatmentType::MEDICALERROR);
	card->id = counter;
	tmpDeck.push_back(card);
	counter++;

	FillDeck(tmpDeck);
}

std::vector<Card*> Maze::DealCards(unsigned int maxCardsToDeal)
{
	std::vector<Card*> tmpHand;
	
	
	for (int i = 0; i < maxCardsToDeal; i++)
	{
		if (deck.size() > 0)
		{
			tmpHand.push_back(deck.top());
			deck.pop();
		}
		else
		{
			std::vector<Card*> tmpDiscard;
			while (!discardDeck.empty())
			{
				tmpDiscard.push_back(discardDeck.top());
				discardDeck.pop();
			}
			FillDeck(tmpDiscard);
			i--;
		}

	}
	return tmpHand;
}

void Maze::FillDeck(std::vector<Card*> tmpDeck)
{
	// RANDOMIZE MAZE
	std::random_shuffle(tmpDeck.begin(), tmpDeck.end());
	
	// ADD TO THE CARDS STACK
	for (int i = 0; i < tmpDeck.size(); i++)
	{
		deck.push(tmpDeck[i]);
	}
}

void Maze::DiscardCard(Player& p, Card *c, int id)
{
	// DISCARD THE CARD USED
// Add to discard cards
	p.maze->discardDeck.push(c);
	// Delete from the hand
	p.hand.erase(p.hand.begin() + id);
	// Draw new card
	std::vector<Card*> tmpCards = p.maze->DealCards(1);
	for (Card* c : tmpCards)
	{
		p.hand.push_back(c);
		std::cout << "You drawn: "; c->Draw(); std::cout << std::endl;
	}
}

void Maze::DiscardOtherCard(Player& p, Card* c, int i, int id)
{
	// Discard card used
	// Add the card played for other player & erase from his hand, also he needs to draw a new one
	p.maze->discardDeck.push(p.otherhands.at(i).at(id));
	p.otherhands.at(i).erase(p.otherhands.at(i).begin() + id);
	std::vector<Card*> tmpCards = p.maze->DealCards(1);
	for (Card* c : tmpCards)
	{
		p.otherhands.at(i).push_back(c);
	}
}

void Maze::DiscardAllOtherHandCards(Player& p)
{
	for (auto c : p.otherhands)
	{
		for (auto c2 : c)
		{
			discardDeck.push(c2);
		}
	}
	for (int i = 0; i < p.otherhands.size(); i++)
	{
		p.otherhands.at(i).clear();
	}
}

void Maze::DiscardAllOtherHandCards(Player& p, int playerThatUsedCard)
{
	for (int i = 0; i < p.otherhands.size(); i++)
	{
		if (i != playerThatUsedCard)
		{
			for (int j = 0; j < p.otherhands.at(i).size(); j++)
			{
				discardDeck.push(p.otherhands.at(i).at(j));
			}
		}
	}
	for (int i = 0; i < p.otherhands.size(); i++)
	{
		if (i != playerThatUsedCard)
		{
			p.otherhands.at(i).clear();
		}
	}
}

void Maze::DiscardAllHandCards(Player& p)
{
	for (auto c : p.hand)
	{
		discardDeck.push(c);
	}

	p.hand.clear();
}
