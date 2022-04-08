#pragma once
#include <vector>
#include "Card.h"
#include "Maze.h"

struct OtherPlayerCards
{
	int idPlayer;
	std::vector<Card*> hand;
	
	OtherPlayerCards();
	OtherPlayerCards(int _idPlayer, std::vector<Card*> _hand) : idPlayer(_idPlayer), hand(_hand)  {};
};

class Player
{
public:
	
	unsigned int randomSeed;
	
	// id
	int id;
	std::vector<int> idOtherPlayers;
	
	// game turn
	int idTurn;
	std::vector<int> idOtherTurns;
	
	// card games variables
	std::vector<Card*> hand;
	std::vector<std::vector<Card*>> otherhands;
	
	// played cards
	std::vector<Card*> playedCards;
	std::vector<std::vector<Card*>> otherPlayedCards;
	
	// maze
	Maze* maze;

	Player();
	~Player();

	int FindPlayerInOtherIdPlayers(int id);
	// Iterators find positions in containers by id card
	
	// CARDS
	// Hands
	int FindPositionCardbyIDCardInHand(int cardID); // My Hand
	int FindPositionCardbyIDCardInOtherHands(int cardID); // Other Hands
	
	// Played Cards
	int FindPositionCardbyIDCardInPlayedCards(int cardID); // My played cards
	Card* FindCardbyIDCardInPlayedCards(int cardID); // My played cards
	int FindPositionCardbyIDCardInOtherPlayedCards(int cardID); // Other played cards
	
	// PLAYER
	// Hands
	int FindPositionPlayerbyIDCardInOtherHands(int cardID); // Other played cards
	// Played Cards
	int FindPositionPlayerbyIDCardInOtherPlayedCards(int cardID); // Other played cards
};