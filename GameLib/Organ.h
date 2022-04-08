#pragma once
#include "Card.h"
class Organ : public Card
{
public:
	enum class EOrganType { JOKER, HEART, STOMACH, BRAIN, BONE };
	EOrganType type;

	Organ();
	Organ(EOrganType _type);

	void Draw();
	void Play(Player& p, Card* cardToEffect, int id);
	
	Organ::EOrganType GetType(Organ *organ);
};