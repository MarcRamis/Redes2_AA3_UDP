#include "Protocol.h"

void Protocol::Peer::AckPassword(TcpSocket* client, std::string& gameName)
{
	std::string txtPassword = " ";
	std::cout << "Write the correct password: " << std::endl;
	std::cin >> txtPassword;

	OutputMemoryStream pack;
	pack.Write(static_cast<int>(Protocol::PEER_BSSProtocol::ACK_PWD));
	pack.WriteString(gameName);
	pack.WriteString(txtPassword);
	Status status = client->Send(pack);
	if (status.GetStatus() != Status::EStatusType::DONE)
	{
		std::cout << "No se envio la password" << std::endl;
	}
}

void Protocol::Peer::Chat(std::vector<TcpSocket*>* _clientes, bool& isChat)
{
	std::string opc;

	while (opc != "exit")
	{
		if (isChat)
		{
			std::cout << "Write a message: " << std::endl;
			std::getline(std::cin, opc);

			OutputMemoryStream pack;
			pack.Write(static_cast<int>(Protocol::PEER_PEERProtocol::SENDMESSAGE));
			pack.WriteString(opc);

			for (size_t i = 0; i < _clientes->size(); i++)
			{
				Status status = (_clientes->at(i))->Send(pack);
				if (status.GetStatus() != Status::EStatusType::DONE)
				{
					std::cout << "El mensaje enviado Peer2Peer no se ha enviado: " << std::endl;
				}
			}
		}

	}
}

void Protocol::Peer::SendPlayOrgan(std::vector<TcpSocket*>* _clientes, int idPlayerThatUsedCard, int idCardPlayed)
{
	OutputMemoryStream pack;
	for (int i = 0; i < _clientes->size(); i++)
	{
		pack.Write(static_cast<int>(Protocol::PEER_PEERProtocol::PLAYORGAN));

		pack.Write(idPlayerThatUsedCard); pack.Write(idCardPlayed); // id player that used a card & the position of his card

		Status status = _clientes->at(i)->Send(pack);
		if (status.GetStatus() != Status::EStatusType::DONE)
		{
			std::cout << "El mensaje Peer2Peer no se ha enviado: [ No se ha enviado la carta jugada ]" << std::endl;
		}
	}
}

void Protocol::Peer::ReceivedPlayedOrgan(std::vector<TcpSocket*>* _clientes, InputMemoryStream pack, Player& p)
{
	// Read all ids
	int idPlayerThatUsedCard = 0; int idCardPlayed = 0;
	pack.Read(&idPlayerThatUsedCard); pack.Read(&idCardPlayed);

	int i = p.FindPlayerInOtherIdPlayers(idPlayerThatUsedCard);
	// Add the card played for other player & erase from his hand, also he needs to draw a new one 
	p.otherPlayedCards.at(i).push_back(p.otherhands.at(i).at(idCardPlayed));
	p.otherhands.at(i).erase(p.otherhands.at(i).begin() + idCardPlayed);
	std::vector<Card*> tmpCards = p.maze->DealCards(1);
	for (Card* c : tmpCards)
	{
		p.otherhands.at(i).push_back(c);
	}

}

void Protocol::Peer::SendInfectOrgan(std::vector<TcpSocket*>* _clientes, int idPlayerThatUsedCard, int idCardPlayed, int idCardFromPlayerAffected)
{
	OutputMemoryStream pack;
	for (int i = 0; i < _clientes->size(); i++)
	{
		pack.Write(static_cast<int>(Protocol::PEER_PEERProtocol::INFECTORGAN));

		pack.Write(idPlayerThatUsedCard); pack.Write(idCardPlayed); // id player that used a card & the position of his card
		pack.Write(idCardFromPlayerAffected); // id player that the card has affected

		Status status = _clientes->at(i)->Send(pack);
		if (status.GetStatus() != Status::EStatusType::DONE)
		{
			std::cout << "El mensaje Peer2Peer no se ha enviado: [ No se ha enviado la carta jugada ]" << std::endl;
		}
	}
}

void Protocol::Peer::ReceivedOrganInfected(std::vector<TcpSocket*>* _clientes, InputMemoryStream pack, Player& p)
{
	// Read all ids
	int idPlayerThatUsedCard = 0; int idCardPlayed = 0;
	int idCardFromPlayerAffected = 0;
	pack.Read(&idPlayerThatUsedCard); pack.Read(&idCardPlayed); // receive player & card id used
	pack.Read(&idCardFromPlayerAffected); // receive player & card id who affect this card

	// search if the player is me
	int id_card = p.FindPositionCardbyIDCardInPlayedCards(idCardFromPlayerAffected);
	
	if (id_card != -1)
	{
		int i = p.FindPlayerInOtherIdPlayers(idPlayerThatUsedCard);
		
		p.playedCards.at(id_card)->GetVirusCard()->InfectMe(p, p.playedCards.at(id_card), id_card);
		p.maze->DiscardOtherCard(p, p.otherhands.at(i).at(idCardPlayed), i, idCardPlayed);
	}
	else
	{
		id_card = p.FindPositionCardbyIDCardInOtherPlayedCards(idCardFromPlayerAffected);
		int id_player = p.FindPositionPlayerbyIDCardInOtherPlayedCards(idCardFromPlayerAffected);
		int i = p.FindPlayerInOtherIdPlayers(idPlayerThatUsedCard);
		
		p.otherPlayedCards.at(id_player).at(id_card)->GetVirusCard()->Infect(p, p.otherPlayedCards.at(id_player).at(id_card), id_player, id_card);
		p.maze->DiscardOtherCard(p, p.otherhands.at(i).at(idCardPlayed), i, idCardPlayed);
	}
}

void Protocol::Peer::SendMedicineCard(std::vector<TcpSocket*>* _clientes, int idPlayerThatUsedCard, int idCardPlayed, int idCardFromPlayerAffected)
{
	OutputMemoryStream pack;
	for (int i = 0; i < _clientes->size(); i++)
	{
		pack.Write(static_cast<int>(Protocol::PEER_PEERProtocol::VACUNATEORGAN));

		pack.Write(idPlayerThatUsedCard); pack.Write(idCardPlayed); // id player that used a card & the position of his card
		pack.Write(idCardFromPlayerAffected); // id player that the card has affected & the id of his card

		Status status = _clientes->at(i)->Send(pack);
		if (status.GetStatus() != Status::EStatusType::DONE)
		{
			std::cout << "El mensaje Peer2Peer no se ha enviado: [ No se ha enviado la carta jugada ]" << std::endl;
		}
	}
}

void Protocol::Peer::ReceivedMedicineCard(std::vector<TcpSocket*>* _clientes, InputMemoryStream pack, Player& p)
{
	// Read all ids
	int idPlayerThatUsedCard = 0; int idCardPlayed = 0;
	int idCardFromPlayerAffected = 0;
	pack.Read(&idPlayerThatUsedCard); pack.Read(&idCardPlayed); // receive player & card id used
	pack.Read(&idCardFromPlayerAffected); // receive player & card id who affect this card
	
	int id_card = p.FindPositionCardbyIDCardInOtherPlayedCards(idCardFromPlayerAffected);
	int id_player = p.FindPositionPlayerbyIDCardInOtherPlayedCards(idCardFromPlayerAffected);
	int i = p.FindPlayerInOtherIdPlayers(idPlayerThatUsedCard);

	p.otherPlayedCards.at(id_player).at(id_card)->GetMedicineCard()->Vacunate(p.otherPlayedCards.at(id_player).at(id_card));
	p.maze->DiscardOtherCard(p, p.otherhands.at(i).at(idCardPlayed), i, idCardPlayed);
}

void Protocol::Peer::SendDiscardCard(std::vector<TcpSocket*>* _clientes, int idPlayerThatDiscard, int numberCards)
{
	OutputMemoryStream pack;
	pack.Write(static_cast<int>(Protocol::PEER_PEERProtocol::DISCARDCARDS));

	pack.Write(idPlayerThatDiscard); pack.Write(numberCards); // id player that discard & the number of cards

	for (int i = 0; i < _clientes->size(); i++)
	{
		Status status = _clientes->at(i)->Send(pack);
		if (status.GetStatus() != Status::EStatusType::DONE)
		{
			std::cout << "El mensaje Peer2Peer no se ha enviado: [ No se ha enviado la carta jugada ]" << std::endl;
		}
	}
}

void Protocol::Peer::ReceivedDiscardCard(std::vector<TcpSocket*>* _clientes, InputMemoryStream pack, Player& p)
{
	// Read all ids
	int idPlayerThatDiscard = 0; int numberOfCards = 0;
	pack.Read(&idPlayerThatDiscard); pack.Read(&numberOfCards);

	for (int i = 0; i < p.idOtherPlayers.size(); i++)
	{
		if (p.idOtherPlayers.at(i) == idPlayerThatDiscard)
		{

			for (int j = 0; j < numberOfCards; j++)
			{
				p.maze->discardDeck.push(p.otherhands.at(i).at(j));
			}
			for (int j = 0; j < numberOfCards; j++)
			{
				p.otherhands.at(i).erase(p.otherhands.at(i).begin());
			}
			std::vector<Card*> tmpCards = p.maze->DealCards(numberOfCards);
			for (Card* c : tmpCards)
			{
				p.otherhands.at(i).push_back(c);
			}

		}
	}
}

void Protocol::Peer::YouLost(std::vector<TcpSocket*>* _clientes)
{
	OutputMemoryStream pack; 

	for (int i = 0; i < _clientes->size(); i++)
	{
		pack.Write(static_cast<int>(Protocol::PEER_PEERProtocol::YOULOST));
		Status status = _clientes->at(i)->Send(pack);
		if (status.GetStatus() != Status::EStatusType::DONE)
		{
			std::cout << "No se envio el protocolo que indica a los jugadores que han perdido" << std::endl;
		}
	}
}

void Protocol::Peer::SendLatexGlove(std::vector<TcpSocket*>* _clientes, int idPlayerThatUsedCard, int idCardPlayed, int gameTurn)
{
	OutputMemoryStream pack;
	for (int i = 0; i < _clientes->size(); i++)
	{
		pack.Write(static_cast<int>(Protocol::PEER_PEERProtocol::LATEXGLOVE));

		pack.Write(idPlayerThatUsedCard); pack.Write(idCardPlayed); // id player that used a card & the position of his card
		pack.Write(gameTurn); // game turn

		Status status = _clientes->at(i)->Send(pack);
		if (status.GetStatus() != Status::EStatusType::DONE)
		{
			std::cout << "El mensaje Peer2Peer no se ha enviado: [ No se ha enviado la carta jugada ]" << std::endl;
		}
	}
}

void Protocol::Peer::ReceivedLatexGlove(std::vector<TcpSocket*>* _clientes, InputMemoryStream pack, Player& p)
{
	// Read all ids
	int idPlayerThatUsedCard = 0; int idCardPlayed = 0; int gameTurn = 0;
	pack.Read(&idPlayerThatUsedCard); pack.Read(&idCardPlayed);
	pack.Read(&gameTurn);

	// Discard all my cards
	p.maze->DiscardAllHandCards(p);
	
	// Discard all the cards in the hand of other players
	int j = p.FindPlayerInOtherIdPlayers(idPlayerThatUsedCard);
	for (int i = 0; i < p.otherPlayedCards.size(); i++)
	{
		if (i != j)
		{
			p.maze->DiscardAllOtherHandCards(p, j);
		}
	}

	// Discard the card used
	p.maze->DiscardOtherCard(p, p.otherhands.at(j).at(idCardPlayed), j, idCardPlayed);
	
	// Draw 3 cards because all hands have been discarded & now they have to draw in order of his turn on gameTurn
	bool allDraw = false;
	bool meDraw = false;
	bool doOnceMe = false;
	std::vector<bool> otherPlayerDraw(_clientes->size(), false);
	std::vector<bool> otherdoOnce(_clientes->size(), false);
	std::vector<bool> allPlayerDraw;
	
	while(!allDraw)
	{
		if (gameTurn == p.idTurn)
		{
			if (!meDraw)
			{
				std::vector<Card*> tmpCards1 = p.maze->DealCards(3);
				for (Card* c : tmpCards1)
				{
					p.hand.push_back(c);
					std::cout << "You drawn: "; c->Draw(); std::cout << std::endl;
				}
				meDraw = true;
			}
			if (gameTurn != _clientes->size()) gameTurn++;
			else gameTurn = 0;
		}
		else
		{
			for (int i = 0; i < p.idOtherTurns.size(); i++)
			{
				if (gameTurn == p.idOtherTurns.at(i))
				{
					if (i != j)
					{
						if (!otherPlayerDraw.at(i))
						{
							std::vector<Card*> tmpCards = p.maze->DealCards(3);
							for (Card* c : tmpCards)
							{
								p.otherhands.at(i).push_back(c);
							}
							otherPlayerDraw.at(i) = true;
						}
					}
					if (gameTurn != _clientes->size()) gameTurn++;
					else gameTurn = 0;
				}
			}
		}

		if (meDraw) {
			if (!doOnceMe)
			{
				allPlayerDraw.push_back(meDraw);
				doOnceMe = true;
			}
		}
		// for para saber si todos siguen en falso
		for (int i = 0; i < otherPlayerDraw.size(); i++)
		{
			if (i != j)
			{
				if (otherPlayerDraw.at(i))
				{
					if (!otherdoOnce.at(i))
					{
						allPlayerDraw.push_back(otherPlayerDraw.at(i));
						otherdoOnce.at(i) = true;
					}
				}
			}
		}
		if (allPlayerDraw.size() == _clientes->size())
		{
			allDraw = true;
		}
	}
}

void Protocol::Peer::SendOrganThief(std::vector<TcpSocket*>* _clientes, int idPlayerThatUsedCard, int idCardPlayed, int idCardFromPlayerAffected)
{
	OutputMemoryStream pack;
	for (int i = 0; i < _clientes->size(); i++)
	{
		pack.Write(static_cast<int>(Protocol::PEER_PEERProtocol::ORGANTHIEF));

		pack.Write(idPlayerThatUsedCard); pack.Write(idCardPlayed); // id player that used a card & the position of his card
		pack.Write(idCardFromPlayerAffected); // id player that the card has affected

		Status status = _clientes->at(i)->Send(pack);
		if (status.GetStatus() != Status::EStatusType::DONE)
		{
			std::cout << "El mensaje Peer2Peer no se ha enviado: [ No se ha enviado la carta jugada ]" << std::endl;
		}
	}
}

void Protocol::Peer::ReceivedOrganThief(std::vector<TcpSocket*>* _clientes, InputMemoryStream pack, Player& p)
{
	// Read all ids
	int idPlayerThatUsedCard = 0; int idCardPlayed = 0; int idCardFromPlayerAffected = 0;
	pack.Read(&idPlayerThatUsedCard); pack.Read(&idCardPlayed);
	pack.Read(&idCardFromPlayerAffected);
	
	int id_card = p.FindPositionCardbyIDCardInPlayedCards(idCardFromPlayerAffected); // find player that has been affected by the card
	int i = p.FindPlayerInOtherIdPlayers(idPlayerThatUsedCard); // find player that used the card

	// search if the player is me
	if (id_card != -1)
	{
		// Push to that other played cards
		p.otherPlayedCards.at(i).push_back(p.playedCards.at(id_card));
		// Erase to my played cards
		p.playedCards.erase(p.playedCards.begin() + id_card);
	}
	else
	{
		id_card = p.FindPositionCardbyIDCardInOtherPlayedCards(idCardFromPlayerAffected);
		int id_player = p.FindPositionPlayerbyIDCardInOtherPlayedCards(idCardFromPlayerAffected);
		
		// Push to the player that stole it
		p.otherPlayedCards.at(i).push_back(p.otherPlayedCards.at(id_player).at(id_card));
		// Erase from that other player that the first one stole the organ
		p.otherPlayedCards.at(id_player).erase(p.otherPlayedCards.at(id_player).begin() + id_card);
	}

	// Discard the used card
	p.maze->DiscardOtherCard(p, p.otherhands.at(i).at(idCardPlayed), i, idCardPlayed);
}