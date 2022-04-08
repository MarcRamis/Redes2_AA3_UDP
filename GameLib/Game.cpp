#include "Game.h"

void Game::LoopGame(std::vector<TcpSocket*>* _clientes, Player& player)
{
	while (!gameEnd && !WinCondition(_clientes, player))
	{
		if (gameTurn == player.idTurn)
		{
			canChat = false;
			PlayCard(_clientes, player);
		}
		else
		{
			canChat = true;
		}
	}

}

void Game::DrawGame(std::vector<TcpSocket*>* _clientes, Player& player)
{
	ConsoleClear();

	// HUD --> (NAME ID + TURN)
	// Draw me
	ConsoleXY(0, 0);
	ConsoleSetColor(ConsoleColor::DARKGREEN, ConsoleColor::BLACK);
	std::cout << localPort << " | " << player.idTurn + 1 << std::endl;
	ConsoleSetColor(ConsoleColor::WHITE, ConsoleColor::BLACK);

	// Player game turn
	ConsoleXY(HUD_MAX_POS_GAME_X / 2, HUD_MAX_POS_GAME_Y / 2);
	std::cout << " Player Turn: ";
	ConsoleXY((HUD_MAX_POS_GAME_X / 2) - 1, (HUD_MAX_POS_GAME_Y / 2) + 1);
	if (gameTurn == player.idTurn)
	{
		std::cout << " [ " << localPort << " | " << gameTurn + 1 << " ] " << std::endl;
	}
	else
	{
		for (int i = 0; i < _clientes->size(); i++)
		{
			if (gameTurn == player.idOtherTurns.at(i))
			{
				std::cout << " [ " << _clientes->at(i)->GetRemotePort().port << " | " << gameTurn + 1 << " ] " << std::endl;
				break;
			}
		}
	}

	// Draw other players
	for (int i = 0; i < _clientes->size(); i++)
	{
		if (i == 0)
		{
			ConsoleXY(HUD_MAX_POS_GAME_X, 0);
			ConsoleSetColor(ConsoleColor::DARKRED, ConsoleColor::BLACK);
		}
		else if (i == 1)
		{
			ConsoleXY(0, HUD_MAX_POS_GAME_Y);
			ConsoleSetColor(ConsoleColor::DARKRED, ConsoleColor::BLACK);
		}
		else if (i == 2)
		{
			ConsoleXY(HUD_MAX_POS_GAME_X, HUD_MAX_POS_GAME_Y);
			ConsoleSetColor(ConsoleColor::DARKRED, ConsoleColor::BLACK);
		}
		std::cout << _clientes->at(i)->GetRemotePort().port << " | " << player.idOtherTurns.at(i) + 1 << std::endl;
		ConsoleSetColor(ConsoleColor::WHITE, ConsoleColor::BLACK);
	}

	// HUD --> (PLAYER CARDS)
	// Draw this played cards
	int countRow = 1;
	for (Card* c : player.playedCards)
	{
		ConsoleXY(0, countRow);
		std::cout << c->id << " - ";
		c->Draw();
		countRow++;
	}

	// Draw other played cards
	for (int i = 0; i < player.otherPlayedCards.size(); i++)
	{
		if (i == 0)
		{
			countRow = 1;
		}
		else if (i == 1)
		{
			countRow = HUD_MAX_POS_GAME_Y - 1;
		}
		else if (i == 2)
		{
			countRow = HUD_MAX_POS_GAME_Y - 1;
		}

		for (int j = 0; j < player.otherPlayedCards.at(i).size(); j++)
		{
			if (i == 0)
			{
				ConsoleXY(HUD_MAX_POS_GAME_X, countRow);
				countRow++;
			}
			else if (i == 1)
			{
				ConsoleXY(0, countRow);
				countRow--;
			}
			else if (i == 2)
			{
				ConsoleXY(HUD_MAX_POS_GAME_X, countRow);
				countRow--;
			}
			std::cout << player.otherPlayedCards.at(i).at(j)->id << " - ";
			player.otherPlayedCards.at(i).at(j)->Draw();
			std::cout << std::endl;
		}
	}

	// HUD --> (DECK WHERE DRAW)
	ConsoleXY(HUD_MAX_POS_GAME_X + 20, 0);
	ConsoleSetColor(ConsoleColor::DARKYELLOW, ConsoleColor::BLACK);
	std::cout << "- | Deck[ " << player.maze->deck.size() << " ]| -" << std::endl;

	// HUD --> (DISCARD DECK)
	std::stack<Card*> printDiscardDeck = player.maze->discardDeck;
	ConsoleXY(HUD_MAX_POS_GAME_X + 40, 0);
	ConsoleSetColor(ConsoleColor::DARKYELLOW, ConsoleColor::BLACK);
	std::cout << "- | Discard Deck[ " << player.maze->discardDeck.size() << " ]| -" << std::endl;
	countRow = 1;
	while (!printDiscardDeck.empty())
	{
		ConsoleXY(HUD_MAX_POS_GAME_X + 45, countRow);
		printDiscardDeck.top()->Draw();
		countRow++;
		printDiscardDeck.pop();
	}

	// HUD --> (MY HAND)
	ConsoleSetColor(ConsoleColor::WHITE, ConsoleColor::BLACK);
	ConsoleXY(0, HUD_MAX_POS_GAME_Y + 2);
	std::cout << "My hand: " << std::endl;
	for (Card* c : player.hand)
	{
		c->Draw();
	}
	std::cout << std::endl << std::endl;

	// HUD --> (OTHER HANDS)
	//for (auto c : player.otherhands)
	//{
	//	std::cout << "Other hands: " << std::endl;
	//	for (auto c2 : c)
	//	{
	//		c2->Draw();
	//	}
	//	std::cout << std::endl;
	//}
}

void Game::PlayCard(std::vector<TcpSocket*>* _clientes, Player& player)
{
	bool endTurn = true;
	Card* tmpCard = new Card();
	int selection = 0;
	int selectionToAffect = 0;

	while (endTurn)
	{
		selection = 0;
		do {
			std::cout << "Use one of your cards: ( 1 - 3 ) or Discard ( 4 )" << std::endl;
			std::cin >> selection;
		} while (selection > 4 || selection < 1);

		// Play cards
		if (selection <= 3 && selection >= 1)
		{
			tmpCard = player.hand.at(selection - 1);

			switch (player.hand.at(selection - 1)->cardType)
			{
			case Card::EType::ORGAN:

				if (OrganAlreadyExistsInTable(player, player.hand.at(selection - 1)))
				{
					std::cout << "This card is already played" << std::endl;
				}
				else
				{
					player.hand.at(selection - 1)->Play(player, nullptr, selection - 1);
					Protocol::Peer::SendPlayOrgan(_clientes, player.id, selection - 1); // send protocol to modify other players 
					endTurn = !endTurn;
				}
				break;
			case Card::EType::MEDICINE:

				selectionToAffect = 0;
				do {
					std::cout << "Select a card on the table: ( Any of the number next to the card ) or (-1) to exit if there is no organ to infect" << std::endl;
					std::cin >> selectionToAffect;

					if (selectionToAffect != -1)
					{
						if (!CardMedicineIsTheSameType(player, player.hand.at(selection - 1), player.FindCardbyIDCardInPlayedCards(selectionToAffect)))
						{
							std::cout << "That's not a valid card" << std::endl;
						}
						else
						{
							std::cout << "Valid card" << std::endl;
							break;
						}
					}

				} while (!CorrectIdCardInTable(selectionToAffect, player) && selectionToAffect != -1);

				if (selectionToAffect != -1) {
					if (CardMedicineIsTheSameType(player, player.hand.at(selection - 1), player.FindCardbyIDCardInPlayedCards(selectionToAffect)))
					{
						player.hand.at(selection - 1)->VacunateOrgan(player, player.FindPositionCardbyIDCardInPlayedCards(selectionToAffect), selection - 1);
						Protocol::Peer::SendMedicineCard(_clientes, player.id, selection - 1, selectionToAffect); // send protocol to modify other players 
						endTurn = !endTurn;
					}
				}

				break;
			case Card::EType::TREATMENT:

				switch (player.hand.at(selection - 1)->GetTreatmentCard()->type)
				{
				case Treatment::ETreatmentType::INFECTION:

					if (selectionToAffect != -1)
					{
						if (player.otherPlayedCards
							.at(GetIDFromSelectedPlayer(player, selectionToAffect))
							.at(GetIDFromSelectedCard(player, selectionToAffect))->state != Card::EOrganState::IMMUNIZED)
						{
							
						}
					}
					else {
						std::cout << "This card is immunized" << std::endl;
					}
					break;
				case Treatment::ETreatmentType::ORGANTHIEF:
					
					do {
						std::cout << "Select a card on the table: ( Any of the number next to the card ) or (-1) to exit if there is no organ to steal" << std::endl;
						std::cin >> selectionToAffect;

					} while (!CorrectIdCardInTable(selectionToAffect, player) && selectionToAffect != -1);

					if (selectionToAffect != -1)
					{
						if (player.otherPlayedCards
							.at(GetIDFromSelectedPlayer(player, selectionToAffect))
							.at(GetIDFromSelectedCard(player, selectionToAffect))->state != Card::EOrganState::IMMUNIZED)
						{
							if (OrganAlreadyExistsInTable(player, player.otherPlayedCards
								.at(GetIDFromSelectedPlayer(player, selectionToAffect))
								.at(GetIDFromSelectedCard(player, selectionToAffect))))
							{
								std::cout << "This card is already played" << std::endl;
							}
							else
							{
								player.hand.at(selection - 1)->GetTreatmentCard()->PlayOrganThief(player, selection - 1,
									GetIDFromSelectedPlayer(player, selectionToAffect), GetIDFromSelectedCard(player, selectionToAffect));
								Protocol::Peer::SendOrganThief(_clientes, player.id, selection - 1, selectionToAffect); // send protocol to modify other players 
								endTurn = !endTurn;
							}
						}
					}
					else {
						std::cout << "This card is immunized" << std::endl;
					}

					break;
				case Treatment::ETreatmentType::TRASPLANT:
					if (player.hand.at(selection - 1)->state == Card::EOrganState::IMMUNIZED)
					{

					}
					else {
						std::cout << "This card is immunized" << std::endl;
					}
					break;
				case Treatment::ETreatmentType::LATEXGLOVE:
					player.hand.at(selection - 1)->GetTreatmentCard()->PlayLatexGlove(player, selection - 1);
					Protocol::Peer::SendLatexGlove(_clientes, player.id, selection - 1, gameTurn); // send protocol to modify other players 
					endTurn = !endTurn;
					latexGloveEnd = true;
					break;
				case Treatment::ETreatmentType::MEDICALERROR:
					break;
				default:
					break;
				}
				break;
			case Card::EType::VIRUS:

				selectionToAffect = 0;
				do {
					std::cout << "Select a card on the table: ( Any of the number next to the card ) or (-1) to exit if there is no organ to infect" << std::endl;
					std::cin >> selectionToAffect;

					if (selectionToAffect != -1)
					{
						if (!CardVirusIsTheSameType(player, player.hand.at(selection - 1), GetCardFromSelectedCard(player, selectionToAffect)))
						{
							std::cout << "That's not a valid card" << std::endl;
						}
						else
						{
							std::cout << "Valid card" << std::endl;
							break;
						}
					}


				} while (!CorrectIdCardInTable(selectionToAffect, player) && selectionToAffect != -1);

				if (selectionToAffect != -1) {
					if (CardVirusIsTheSameType(player, player.hand.at(selection - 1), GetCardFromSelectedCard(player, selectionToAffect)))
					{
						player.hand.at(selection - 1)->InfectOrgan(player, GetIDFromSelectedPlayer(player, selectionToAffect), GetIDFromSelectedCard(player, selectionToAffect), selection - 1);
						Protocol::Peer::SendInfectOrgan(_clientes, player.id, selection - 1, selectionToAffect); // send protocol to modify other players 
						endTurn = !endTurn;
					}
				}

				break;
			default:
				break;
			}
			if (!endTurn)
			{
				if (latexGloveEnd)
				{
					for (int i = 0; i < _clientes->size() + 1; i++)
					{
						NextTurnGlove(_clientes, player);
					}

					ConsoleWait(2000.f);
					DrawGame(_clientes, player);
					latexGloveEnd = false;
				}
				else
				{
					NextTurn(_clientes, player);
				}
			}

		}
		else
		{
			selectionToAffect = 0;
			do {
				std::cout << "Select the number of cards you want to discard(1 - 3) or (-1) to exit" << std::endl;
				std::cin >> selectionToAffect;
			} while ((selectionToAffect < 1 || selectionToAffect > 3) && selectionToAffect != -1);
			if (selectionToAffect != -1)
			{
				for (int i = 0; i < selectionToAffect; i++)
				{
					player.maze->discardDeck.push(player.hand.at(i));
				}
				for (int i = 0; i < selectionToAffect; i++)
				{
					player.hand.erase(player.hand.begin());
				}
				std::vector<Card*> tmpCards = player.maze->DealCards(selectionToAffect);
				for (Card* c : tmpCards)
				{
					player.hand.push_back(c);
					std::cout << "You drawn: "; c->Draw(); std::cout << std::endl;
				}
				Protocol::Peer::SendDiscardCard(_clientes, player.id, selectionToAffect);

				endTurn = !endTurn;

				NextTurn(_clientes, player);
			}

		}

	}
	DrawGame(_clientes, player);
}

void Game::PlayerDisconnected(std::vector<TcpSocket*>* _clientes, Player& player, int i)
{
	for (int j = 0; j < player.otherhands.at(i).size(); j++)
	{
		player.maze->discardDeck.push(player.otherhands.at(i).at(j));
	}
	for (int j = 0; j < player.otherPlayedCards.at(i).size(); j++)
	{
		player.maze->discardDeck.push(player.otherhands.at(i).at(j));
	}

	int tmpTurnPlayerDisconnected = player.idOtherTurns.at(i);

	player.otherhands.erase(player.otherhands.begin() + i);
	player.otherPlayedCards.erase(player.otherPlayedCards.begin() + i);
	player.idOtherTurns.erase(player.idOtherTurns.begin() + i);
	player.idOtherPlayers.erase(player.idOtherPlayers.begin() + i);

	// Get new turn
	if (tmpTurnPlayerDisconnected < player.idTurn) {
		player.idTurn--;
	}
	for (int turn = 0; turn < player.idOtherTurns.size(); turn++)
	{
		if (tmpTurnPlayerDisconnected < player.idOtherTurns.at(turn)) {
			player.idOtherTurns.at(turn)--;
		}
	}

	if (gameTurn == tmpTurnPlayerDisconnected && gameTurn == _clientes->size())
	{
		if (gameTurn != _clientes->size()) gameTurn++;
		else gameTurn = 0;
	}
}

void Game::NextTurn(std::vector<TcpSocket*>* _clientes, Player& player)
{
	ConsoleWait(2000.f);

	if (gameTurn != _clientes->size()) gameTurn++;
	else gameTurn = 0;

	DrawGame(_clientes, player);
}
void Game::NextTurnGlove(std::vector<TcpSocket*>* _clientes, Player& player)
{
	if (gameTurn != _clientes->size()) gameTurn++;
	else gameTurn = 0;
}

void Game::SetTurn(std::vector<TcpSocket*>* _clientes, Player& player)
{
	int myAmmount = 0;
	int tmpAmmount = 0;
	std::vector<std::pair<int, int>> ammountOrgans; // vector where organs counter of every other player are stored
	std::priority_queue<std::pair<int, int> > turnOrgans;

	std::cout << "My initial turn: " << player.idTurn + 1 << std::endl;

	// SUMM ORGANS
	for (Card* c : player.hand)
	{
		if (c->cardType == Card::EType::ORGAN)
		{
			tmpAmmount++;
		}
	}
	ammountOrgans.push_back(std::pair<int, int>(tmpAmmount, player.idTurn));
	tmpAmmount = 0;
	// Count number of organs every other player has
	for (int i = 0; i < player.otherhands.size(); i++)
	{
		for (int j = 0; j < player.otherhands.at(i).size(); j++)
		{
			if (player.otherhands.at(i).at(j)->cardType == Card::EType::ORGAN)
			{
				tmpAmmount++;
			}
		}

		ammountOrgans.push_back(std::pair<int, int>(tmpAmmount, player.idOtherTurns.at(i)));
		tmpAmmount = 0;
	}

	ammountOrgans.size();

	// Me make a reverse loop because we want the first player to be the first one if there is draw in organ cards
	// Ordered by the first one & for max number of organs
	for (int i = 0; i < _clientes->size() + 1; i++)
	{
		turnOrgans.push(std::pair<int, int>(ammountOrgans.at(i).first, ammountOrgans.at(i).second));
	}

	std::vector<std::pair<int, int>> containerTurnOrgans;

	while (!turnOrgans.empty()) {
		std::cout << turnOrgans.top().first
			<< " " << turnOrgans.top().second + 1
			<< std::endl;
		containerTurnOrgans.push_back(std::pair<int, int>(turnOrgans.top().first, turnOrgans.top().second));
		turnOrgans.pop();
	}

	// New turn for this player
	for (int i = 0; i < containerTurnOrgans.size(); i++)
	{
		if (containerTurnOrgans.at(i).second == player.idTurn)
		{
			player.idTurn = i;
			break;
		}
	}
	// Know new turns for other players
	std::vector<int>tmpOtherTurns = player.idOtherTurns;

	for (int i = 0; i < containerTurnOrgans.size(); i++)
	{
		for (int j = 0; j < player.idOtherTurns.size(); j++)
		{
			if (containerTurnOrgans.at(i).second == tmpOtherTurns.at(j))
			{
				player.idOtherTurns.at(j) = i;
				break;
			}
		}
	}

	ConsoleWait(1000.f);
}

bool Game::WinCondition(std::vector<TcpSocket*>* _clientes, Player player)
{
	// Check if this player has won
	int tmpAmmount = 0;
	for (Card* c : player.playedCards)
	{
		if (c->cardType == Card::EType::ORGAN && c->state != Card::EOrganState::INFECTED)
		{
			tmpAmmount++;
		}
	}
	//std::cout << "Me: " << tmpAmmount << std::endl;
	if (tmpAmmount == 4)
	{
		std::cout << "You won! You are such a pro!" << std::endl;
		Protocol::Peer::YouLost(_clientes);
		return true;
	}

	return false;
}

bool Game::CorrectIdCardInTable(int selection, Player& player)
{
	for (int i = 0; i < player.playedCards.size(); i++)
	{
		if (player.playedCards.at(i)->id == selection)
		{
			return true;
		}
	}

	for (int i = 0; i < player.otherPlayedCards.size(); i++)
	{
		for (int j = 0; j < player.otherPlayedCards.at(i).size(); j++)
		{
			if (player.otherPlayedCards.at(i).at(j)->id == selection)
			{
				return true;
			}
		}
	}

	return false;
}

int Game::GetIDFromSelectedPlayer(Player& player, int selection)
{
	for (int i = 0; i < player.otherPlayedCards.size(); i++)
	{
		for (int j = 0; j < player.otherPlayedCards.at(i).size(); j++)
		{
			if (player.otherPlayedCards.at(i).at(j)->id == selection)
			{
				return i;
			}
		}
	}
	return -1;
}

int Game::GetIDFromSelectedCard(Player& player, int selection)
{
	for (int i = 0; i < player.otherPlayedCards.size(); i++)
	{
		for (int j = 0; j < player.otherPlayedCards.at(i).size(); j++)
		{
			if (player.otherPlayedCards.at(i).at(j)->id == selection)
			{
				return j;
			}
		}
	}
	return -1;
}

Card* Game::GetCardFromSelectedCard(Player& player, int selection)
{
	for (int i = 0; i < player.otherPlayedCards.size(); i++)
	{
		for (int j = 0; j < player.otherPlayedCards.at(i).size(); j++)
		{
			if (player.otherPlayedCards.at(i).at(j)->id == selection)
			{
				return player.otherPlayedCards.at(i).at(j);
			}
		}
	}

	return nullptr;
}

bool Game::OrganAlreadyExistsInTable(Player& player, Card* c)
{
	for (int i = 0; i < player.playedCards.size(); i++)
	{
		if (c->GetOrganCard()->type == player.playedCards.at(i)->GetOrganCard()->type) 
		{
			return true;
		}
	}
	return false;
}

bool Game::CardVirusIsTheSameType(Player& player, Card* c, Card* c2)
{
	if (c2->GetOrganCard()->type == Organ::EOrganType::JOKER) {
		return true;
	}
	else
	{
		if (c->GetVirusCard()->type == Virus::EVirusType::VIRUSJOKER)
		{
			return true;
		}
		else
		{
			if (c->GetVirusCard()->type == Virus::EVirusType::VIRUSBONE && c2->GetOrganCard()->type == Organ::EOrganType::BONE)
			{
				return true;
			}
			if (c->GetVirusCard()->type == Virus::EVirusType::VIRUSBRAIN && c2->GetOrganCard()->type == Organ::EOrganType::BRAIN)
			{
				return true;
			}
			if (c->GetVirusCard()->type == Virus::EVirusType::VIRUSHEART && c2->GetOrganCard()->type == Organ::EOrganType::HEART)
			{
				return true;
			}
			if (c->GetVirusCard()->type == Virus::EVirusType::VIRUSSTOMACH && c2->GetOrganCard()->type == Organ::EOrganType::STOMACH)
			{
				return true;
			}
		}
	}

	return false;
}

bool Game::CardMedicineIsTheSameType(Player& player, Card* c, Card* c2)
{
	if (c2->GetOrganCard()->type == Organ::EOrganType::JOKER) {
		return true;
	}
	else
	{
		if (c->GetMedicineCard()->type == Medicine::EMedicineType::MEDICINEJOKER)
		{
			return true;
		}
		else
		{
			if (c->GetMedicineCard()->type == Medicine::EMedicineType::MEDICINEBONE && c2->GetOrganCard()->type == Organ::EOrganType::BONE)
			{
				return true;
			}
			if (c->GetMedicineCard()->type == Medicine::EMedicineType::MEDICINEBRAIN && c2->GetOrganCard()->type == Organ::EOrganType::BRAIN)
			{
				return true;
			}
			if (c->GetMedicineCard()->type == Medicine::EMedicineType::MEDICINEHEART && c2->GetOrganCard()->type == Organ::EOrganType::HEART)
			{
				return true;
			}
			if (c->GetMedicineCard()->type == Medicine::EMedicineType::MEDICINESTOMACH && c2->GetOrganCard()->type == Organ::EOrganType::STOMACH)
			{
				return true;
			}
		}
	}

	return false;
}

void Game::StartGame(std::vector<TcpSocket*>* _clientes, Player& player)
{
	ConsoleClear();
	std::cout << "DEALING CARDS" << std::endl << std::endl;
	ConsoleWait(1000.f);

	srand(player.randomSeed);

	player.maze = new Maze();

	// DEAL INITIAL CARDS
	player.otherPlayedCards.resize(_clientes->size());

	for (int i = 0; i < _clientes->size() + 1; i++)
	{
		if (i == player.idTurn)
		{
			player.id = i; // save this player initial id
			player.hand = player.maze->DealCards(CARDS_TO_DEAL);
		}
		else
		{
			player.idOtherPlayers.push_back(i); // save other players initial id
			player.idOtherTurns.push_back(i); // Add turns from the other players
			std::vector<Card*> cards = player.maze->DealCards(CARDS_TO_DEAL);
			player.otherhands.push_back(cards);
		}
	}

	SetTurn(_clientes, player); // SET NEW TURN
	DrawGame(_clientes, player);

	// UPDATE - LOOP GAME
	LoopGame(_clientes, player);
}