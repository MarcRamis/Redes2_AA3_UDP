#pragma once
#include "Card.h"
class Virus : public Card
{
public:
	enum class EVirusType { VIRUSJOKER, VIRUSHEART, VIRUSSTOMACH, VIRUSBRAIN, VIRUSBONE };
	EVirusType type;

	Virus();
	Virus(EVirusType _type);

	void Draw();
	void Play(Player& p, Card* cardToAffect, int id);

	void InfectOrgan(Player& p, int playerToAffect, int idCardToAffect, int id);
	
	void Infect(Player& p, Card *c, int playerToAffect, int idCardToAffect);
	void InfectMe(Player& p, Card *c, int idCardToAffect);
};

