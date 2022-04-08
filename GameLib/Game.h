#pragma once

#include <vector>
#include <stack>
#include <queue>
#include <string>

#include "TcpSocket.h"
#include "Player.h"
#include "Protocol.h"
#include "Organ.h"
#include "Virus.h"
#include "Medicine.h"
#include "Treatment.h"


class Game
{
public:

	bool gameEnd = false;
	int gameTurn;
	bool canChat;
	bool gameStart;
	bool latexGloveEnd;
	std::vector<bool> gameReady;
	std::string gameName;
	unsigned short localPort;
	
	void StartGame(std::vector<TcpSocket*>* _clientes, Player &player);
	void LoopGame(std::vector<TcpSocket*>* _clientes, Player &player);
	void DrawGame(std::vector<TcpSocket*>* _clientes, Player &player);

	void SetTurn(std::vector<TcpSocket*>* _clientes, Player &player);
	bool WinCondition(std::vector<TcpSocket*>* _clientes, Player player);
	bool CorrectIdCardInTable(int selection, Player& player);
	void NextTurn(std::vector<TcpSocket*>* _clientes, Player& player);
	void NextTurnGlove(std::vector<TcpSocket*>* _clientes, Player& player);
	void PlayCard(std::vector<TcpSocket*>* _clientes, Player& player);
	void PlayerDisconnected(std::vector<TcpSocket*>* _clientes, Player& player, int i);
	
	int GetIDFromSelectedPlayer(Player& player, int selection);
	int GetIDFromSelectedCard(Player& player, int selection);
	Card *GetCardFromSelectedCard(Player& player, int selection);

	bool OrganAlreadyExistsInTable(Player& player, Card* c);
	bool CardVirusIsTheSameType(Player& player, Card* c, Card* c2);
	bool CardMedicineIsTheSameType(Player& player, Card* c, Card* c2);
};