#include "Card.h"
#include "Player.h"
#include "Organ.h"
#include "Virus.h"
#include "Medicine.h"
#include "Treatment.h"

void Card::Draw()
{
}

void Card::Play(Player& p, Card* cardToEffect, int id)
{

}

void Card::InfectOrgan(Player& p, int playerToAffect, int idCardToAffect, int id)
{
}

void Card::VacunateOrgan(Player& p, int idCardToAffect, int id)
{
}

Organ* Card::GetOrganCard()
{
    return dynamic_cast<Organ*>(this);
}

Virus* Card::GetVirusCard()
{
    return dynamic_cast<Virus*>(this);
}

Medicine* Card::GetMedicineCard()
{
    return dynamic_cast<Medicine*>(this);
}

Treatment* Card::GetTreatmentCard()
{
    return dynamic_cast<Treatment*>(this);
}

Card::Card()
{
}

Card::~Card()
{
}
