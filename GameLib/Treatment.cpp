#include "Treatment.h"

#include "Player.h"

Treatment::Treatment()
{
}

Treatment::Treatment(ETreatmentType _type) : type(_type) { cardType = EType::TREATMENT; };

void Treatment::Draw()
{
	{
		switch (type)
		{
		case Treatment::ETreatmentType::INFECTION:
			ConsoleSetColor(ConsoleColor::CYAN, ConsoleColor::BLACK);
			std::cout << "Treatment - Infection | ";
			break;
		case Treatment::ETreatmentType::ORGANTHIEF:
			ConsoleSetColor(ConsoleColor::CYAN, ConsoleColor::BLACK);
			std::cout << "Treatment - Organ Thief | ";
			break;
		case Treatment::ETreatmentType::TRASPLANT:
			ConsoleSetColor(ConsoleColor::CYAN, ConsoleColor::BLACK);
			std::cout << "Treatment - Trasplant | ";
			break;
		case Treatment::ETreatmentType::LATEXGLOVE:
			ConsoleSetColor(ConsoleColor::CYAN, ConsoleColor::BLACK);
			std::cout << "Treatment - Latex glove | ";
			break;
		case Treatment::ETreatmentType::MEDICALERROR:
			ConsoleSetColor(ConsoleColor::CYAN, ConsoleColor::BLACK);
			std::cout << "Treatment - Medical error | ";
			break;
		default:
			break;
		}

		ConsoleSetColor(ConsoleColor::WHITE, ConsoleColor::BLACK);
	}
}

void Treatment::Play(Player& p, Card* cardToEffect, int id)
{
}

void Treatment::PlayLatexGlove(Player& p, int id)
{
	p.maze->DiscardCard(p,this, id);
	p.maze->DiscardAllOtherHandCards(p);

	// Draw 3 cards for other players
	for (int i = 0; i < p.otherPlayedCards.size(); i++)
	{
		std::vector<Card*> tmpCards = p.maze->DealCards(3);
		for (Card* c : tmpCards)
		{
			p.otherhands.at(i).push_back(c);
		}
	}
}

void Treatment::PlayOrganThief(Player& p, int id, int playerToAffect, int idCardAffected)
{
	// Push to my played cards
	p.playedCards.push_back(p.otherPlayedCards.at(playerToAffect).at(idCardAffected));
	// Erase from that other player 
	p.otherPlayedCards.at(playerToAffect).erase(p.otherPlayedCards.at(playerToAffect).begin() + idCardAffected);
	// Discard the used card
	p.maze->DiscardCard(p, this, id);
}