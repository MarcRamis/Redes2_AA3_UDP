#include "Virus.h"
#include "Player.h"

Virus::Virus()
{
}

Virus::Virus(EVirusType _type) : type(_type) { cardType = EType::VIRUS; }

void Virus::Draw()
{
	switch (type)
	{
	case Virus::EVirusType::VIRUSJOKER:
		ConsoleSetColor(ConsoleColor::MAGENTA, ConsoleColor::BLACK);
		std::cout << "Virus - Joker | ";
		break;
	case Virus::EVirusType::VIRUSHEART:
		ConsoleSetColor(ConsoleColor::RED, ConsoleColor::BLACK);
		std::cout << "Virus - Heart | ";
		break;
	case Virus::EVirusType::VIRUSSTOMACH:
		ConsoleSetColor(ConsoleColor::GREEN, ConsoleColor::BLACK);
		std::cout << "Virus - Stomach | ";
		break;
	case Virus::EVirusType::VIRUSBRAIN:
		ConsoleSetColor(ConsoleColor::BLUE, ConsoleColor::BLACK);
		std::cout << "Virus - Brain | ";
		break;
	case Virus::EVirusType::VIRUSBONE:
		ConsoleSetColor(ConsoleColor::DARKYELLOW, ConsoleColor::BLACK);
		std::cout << "Virus - Bone | ";
		break;
	default:
		break;
	}

	ConsoleSetColor(ConsoleColor::WHITE, ConsoleColor::BLACK);
}

void Virus::Play(Player& p, Card* cardToEffect, int id)
{

}

void Virus::InfectOrgan(Player& p, int playerToAffect, int idCardToAffect, int id)
{
	Infect(p, p.otherPlayedCards.at(playerToAffect).at(idCardToAffect), playerToAffect, idCardToAffect);
	p.maze->DiscardCard(p, this, id);
}

void Virus::Infect(Player &p, Card* c, int playerToAffect, int idCardToAffect)
{
	// Infect the card
	switch (c->state)
	{
	case Card::EOrganState::NONE:
		c->state = Card::EOrganState::INFECTED;
		break;
	case Card::EOrganState::INFECTED:
		// If it was infected before, then dies
		c->state = Card::EOrganState::NONE;
		p.maze->discardDeck.push(c); // This add the card to the discard deck
		p.otherPlayedCards.at(playerToAffect).erase(p.otherPlayedCards.at(playerToAffect).begin() + idCardToAffect); // This deletes the card from the table
		break;
	case Card::EOrganState::VACUNATED:
		c->state = Card::EOrganState::NONE;
		break;
	default:
		break;
	}
}

void Virus::InfectMe(Player& p, Card* c, int idCardToAffect)
{
	// Infect the card
	switch (c->state)
	{
	case Card::EOrganState::NONE:
		c->state = Card::EOrganState::INFECTED;
		break;
	case Card::EOrganState::INFECTED:
		// If it was infected before, then dies
		c->state = Card::EOrganState::NONE;
		p.maze->discardDeck.push(c); // This add the card to the discard deck
		p.playedCards.erase(p.playedCards.begin() + idCardToAffect); // This deletes the card from the table
		break;
	case Card::EOrganState::VACUNATED:
		c->state = Card::EOrganState::NONE;
		break;
	default:
		break;
	}
}
