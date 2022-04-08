#include "Player.h"

Player::Player()
{
}

Player::~Player()
{
}

int Player::FindPlayerInOtherIdPlayers(int id)
{
	// this returns the position inside the container of the needed id
	for (int i = 0; i < idOtherPlayers.size(); i++)
	{
		if (idOtherPlayers.at(i) == id)
		{
			return i;
		}
	}

	return -1;
}

int Player::FindPositionCardbyIDCardInHand(int cardID)
{
	for (int i = 0; i < hand.size(); i++)
	{
		if (hand.at(i)->id == cardID)
		{
			return i;
		}
	}
	return -1;
}

int Player::FindPositionCardbyIDCardInOtherHands(int cardID)
{
	for (int i = 0; i < otherhands.size(); i++)
	{
		for (int j = 0; j < otherhands.at(i).size(); j++)
		{		
			if (otherhands.at(i).at(j)->id == cardID)
			{
				return j;
			}
		}
	}

	return -1;
}

int Player::FindPositionCardbyIDCardInPlayedCards(int cardID)
{
	for (int i = 0; i < playedCards.size(); i++)
	{
		if (playedCards.at(i)->id == cardID)
		{
			return i;
		}
	}
	return -1;
}

Card* Player::FindCardbyIDCardInPlayedCards(int cardID)
{
	for (int i = 0; i < playedCards.size(); i++)
	{
		if (playedCards.at(i)->id == cardID)
		{
			return playedCards.at(i);
		}
	}
	return nullptr;
}

int Player::FindPositionCardbyIDCardInOtherPlayedCards(int cardID)
{
	for (int i = 0; i < otherPlayedCards.size(); i++)
	{
		for (int j = 0; j < otherPlayedCards.at(i).size(); j++)
		{
			if (otherPlayedCards.at(i).at(j)->id == cardID)
			{
				return j;
			}
		}
	}

	return -1;
}

int Player::FindPositionPlayerbyIDCardInOtherHands(int cardID)
{
	for (int i = 0; i < otherhands.size(); i++)
	{
		for (int j = 0; j < otherhands.at(i).size(); j++)
		{
			if (otherhands.at(i).at(j)->id == cardID)
			{
				return i;
			}
		}
	}

	return -1;
}

int Player::FindPositionPlayerbyIDCardInOtherPlayedCards(int cardID)
{
	for (int i = 0; i < otherPlayedCards.size(); i++)
	{
		for (int j = 0; j < otherPlayedCards.at(i).size(); j++)
		{
			if (otherPlayedCards.at(i).at(j)->id == cardID)
			{
				return i;
			}
		}
	}

	return -1;
}