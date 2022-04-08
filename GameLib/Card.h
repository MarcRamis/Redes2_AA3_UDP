#pragma once

#include <iostream>
#include <vector>
#include "ConsoleControl.h"

class Player;
class Organ;
class Virus;
class Medicine;
class Treatment;

class Card
{
public:
	
	int id;

	enum class EType { NONE, ORGAN, MEDICINE, VIRUS, TREATMENT };
	EType cardType;

	enum class EOrganState { NONE, IMMUNIZED, VACUNATED, INFECTED };
	EOrganState state = EOrganState::NONE;
	
	Card();
	~Card();
	
	virtual void Draw();
	virtual void Play(Player& p, Card* cardToEffect, int id);
	virtual void InfectOrgan(Player& p, int playerToAffect, int idCardToAffect, int id);
	virtual void VacunateOrgan(Player& p, int idCardToAffect, int id);

	Organ* GetOrganCard();
	Virus* GetVirusCard();
	Medicine* GetMedicineCard();
	Treatment* GetTreatmentCard();
};