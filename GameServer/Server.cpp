#include "Server.h"

bool Server::IsNewClient(unsigned short _clientID)
{
	for (New_Connection *client : new_con_table)
	{
		if (_clientID == client->port) return false; //If It Finds The Client It Means That It Already Existed
	}

	return true;
}

New_Connection* Server::SearchNewClientByPort(unsigned short _clientID)
{
	for (New_Connection *client : new_con_table)
	{
		if (_clientID == client->port) return client;
	}

	return nullptr;
}

Active_Connection* Server::SearchActiveClientByPort(unsigned short _clientID)
{
	for (Active_Connection* client : active_con_table)
	{
		if (_clientID == client->port) return client;
	}

	return nullptr;
}

Active_Connection* Server::SearchActiveClientBySalt(unsigned int _clientCombSalt)
{
	for (Active_Connection* client : active_con_table)
	{
		unsigned int combSalt = client->clientSALT & client->serverSALT;
		if (_clientCombSalt == combSalt)
		{
			return client;
		}
	}

	return nullptr;
}

New_Connection* Server::SearchNewClientBySalt(unsigned int _clientSalt)
{
	for (New_Connection *client : new_con_table)
	{
		unsigned int serSalt = client->clientSALT & client->serverSALT;
		if (_clientSalt == serSalt)
		{
			return client;
		}
	}
	
	return nullptr;
}

void Server::DeleteNewClients(New_Connection _clientToDelete)
{
	int i = 0;

	while (_clientToDelete.port != new_con_table[i]->port)
	{
		i++;
	}

	new_con_table.erase(new_con_table.begin() + i);
}

void Server::DeleteActiveClients(Active_Connection _clientToDelete)
{
	int i = 0;

	while (_clientToDelete.port != active_con_table[i]->port)
	{
		i++;
	}

	active_con_table.erase(active_con_table.begin() + i);
}

void Server::DisconnectClient(int port)
{
	// Disconnects a new client
	if (!IsNewClient(port))
	{
		DeleteNewClients(*SearchNewClientByPort(port));
		std::cout << "Deleted the client with port: " << port << std::endl;
	}
	// Disconnects an active client
	else
	{
		// Send a message to other clients to let them know a player disconnected
		for (Active_Connection* p : active_con_table)
		{
			if (p->port != port)
			{
				std::string messageDisconnect = "--The player with port: " +
					std::to_string(port) + " has been disconnected--";
				
				Send(Protocol::Send(Protocol::STP::CHAT, messageDisconnect, port), p->port);
			}
		}
		
		// Delete if the combined salt coincide with the combined salt client
		DeleteActiveClients(*SearchActiveClientByPort(port));
		std::cout << "Deleted the client with port: " << port << std::endl;
	}
}

void Server::CheckInactivity()
{
	while (isOpen)
	{
		for (New_Connection *nConn : new_con_table)
		{
			if (nConn->TS.ElapsedSeconds() > T_INACTIVITY)
			{
				DisconnectClient(nConn->port);
				Send(Protocol::Send(Protocol::PTS::DISCONNECT_CLIENT), nConn->port);
			}
		}
		for (Active_Connection* aConn : active_con_table)
		{
			if (aConn->TS.ElapsedSeconds() > T_INACTIVITY)
			{
				DisconnectClient(aConn->port);
				Send(Protocol::Send(Protocol::PTS::DISCONNECT_CLIENT), aConn->port);
			}
		}
	}
}

void Server::UpdateClientTimer(int port)
{
	if (!IsNewClient(port))
	{
		if (!new_con_table.empty())
		{
			for (New_Connection* conn : new_con_table)
			{
				if (conn->port == port)
				{
					conn->TS.Start();
				}
			}
		}
	}
	else
	{
		if (!active_con_table.empty())
		{
			for (Active_Connection* conn : active_con_table)
			{
				if (conn->port == port)
				{
					conn->TS.Start();
				}
			}
		}
	}
}

void Server::Send(OutputMemoryStream *pack, int port)
{
	socket->Send(*pack, port);
}

Server::Server()
{
	std::cout << "STARTING THE SERVER" << std::endl;

	socket = new UdpSocket;
	socket->Bind(SERVER_IP);

	std::thread tReceive(&Server::Receive, this);
	tReceive.detach();
	
	std::thread tCheckInactivity(&Server::CheckInactivity, this);
	tCheckInactivity.detach();
}

Server::~Server()
{
	delete socket;
}

UdpSocket* Server::GetSocket()
{
	return socket;
}

bool Server::GetServerOpen()
{
	return isOpen;
}

Game *Server::CreateGame(int _port)
{
	Game* newGame = new Game();
	newGame->GenPlayers(_port);
	games.push_back(newGame);
	
	return newGame;
}

void Server::Receive()
{
	while (isOpen)
	{
		// Receive message
		InputMemoryStream ims = *socket->Receive();
		if (socket->StatusReceived().GetStatus() == Status::EStatusType::DONE)
		{
			//Update client timer
			UpdateClientTimer(socket->PortReceived());

			// Receive header
			int _header; ims.Read(&_header);

			// Find Header
			switch (static_cast<Protocol::PTS>(_header))
			{
			case Protocol::PTS::HELLO_SERVER:
			{
				// Read the message
				std::string messageReceived, ipReceived;
				messageReceived = ims.ReadString(); ipReceived = ims.ReadString();
				unsigned int clientSalt; ims.Read(&clientSalt);
				int _criticId; ims.Read(&_criticId);

				std::cout << socket->PortReceived() << " tells: " << messageReceived << ipReceived << std::endl;

				// Generate Challenge
				int challenge = GenerateChallenge();

				if (IsNewClient(socket->PortReceived()))
				{
					// Create new client
					New_Connection* newClient = new New_Connection(socket->PortReceived(),
						socket->AddressStringReceived(), clientSalt, GenerateSalt(), challenge);
					newClient->TS.Start();
					newClient->name = GenerateName(); // Gen random name
					new_con_table.push_back(newClient);

					// Send critic packet confirmation
					Send(Protocol::Send(Protocol::STP::CRI_PACK_RECEIVED, _criticId), socket->PortReceived());

					// Send New Challenge Request
					Send(Protocol::Send(Protocol::STP::CHALLENGE_REQUEST, challenge,
						newClient->serverSALT), socket->PortReceived());
				}
				else
				{
					// Search for already existing new client
					New_Connection* oldClient = SearchNewClientByPort(socket->PortReceived());

					if (oldClient != nullptr)
					{
						// Send critic packet confirmation
						Send(Protocol::Send(Protocol::STP::CRI_PACK_RECEIVED, _criticId), socket->PortReceived());

						// Send Same Challenge Request
						Send(Protocol::Send(Protocol::STP::CHALLENGE_REQUEST, oldClient->challenge,
							oldClient->serverSALT), socket->PortReceived());
					}
				}
			}

			break;

			case Protocol::PTS::CHALLENGE_RESPONSE:
			{
				// Receive challenge answer
				int challengeResponse;
				ims.Read(&challengeResponse);
				unsigned int saltServerClient; ims.Read(&saltServerClient);
				int _criticId; ims.Read(&_criticId);

				// Identify client
				New_Connection* myClient = SearchNewClientBySalt(saltServerClient);

				if (myClient != nullptr)
				{
					// Challenge answer is correct
					if (myClient->challenge == challengeResponse)
					{
						// Store the new client into active client
						Active_Connection* act_con = new Active_Connection(myClient->port, myClient->address, myClient->clientSALT, myClient->serverSALT);
						act_con->TS.Start();
						act_con->name = myClient->name;
						active_con_table.push_back(act_con);

						// Delete it from the new clients
						DeleteNewClients(*myClient);

						// Send critic packet confirmation
						Send(Protocol::Send(Protocol::STP::CRI_PACK_RECEIVED, _criticId), socket->PortReceived());

						// Send hello message to the 
						Send(Protocol::Send(Protocol::STP::HELLO_CLIENT, act_con->name), myClient->port);
					}
					// Challenge answer is wrong
					else
					{
						myClient->TRY--; // Rest try

						// Check Tries
						if (myClient->TRY > 0)
						{
							// Send Same Challenge Request
							Send(Protocol::Send(Protocol::STP::CHALLENGE_REQUEST,
								myClient->challenge, myClient->serverSALT), myClient->port);
						}
						else //We delete the client
						{
							DeleteNewClients(*myClient);
						}
					}
				}
			}

			break;

			case Protocol::PTS::CHAT:
			{
				// Read the message
				std::string messageReceived = " ";
				messageReceived = ims.ReadString();
				std::cout << socket->PortReceived() << ": " << messageReceived << std::endl;

				// Send it to other clients
				for (Active_Connection* p : active_con_table)
				{
					if (p->port != socket->PortReceived())
					{
						Send(Protocol::Send(Protocol::STP::CHAT,
							messageReceived, socket->PortReceived()), p->port);
					}
				}
			}
			break;

			case Protocol::PTS::DISCONNECT_CLIENT:

				DisconnectClient(socket->PortReceived());
				break;

			case Protocol::PTS::COMMAND:

				// Save list of commands received
				std::cout << "Receiving packages from " << socket->PortReceived() << std::endl;
				
				for (Active_Connection* conn : active_con_table)
				{
					if (conn->port == socket->PortReceived())
					{
						int sizeCommandList; ims.Read(&sizeCommandList);
						for (int i = 0; i < sizeCommandList; i++)
						{
							std::queue<CommandList::EType> tmpCommandListTypes;

							int sizeCommandListTypes; ims.Read(&sizeCommandListTypes);
							
							for (int j = 0; j < sizeCommandListTypes; j++)
							{
								int tmpCommand; ims.Read(&tmpCommand);
								tmpCommandListTypes.push(static_cast<CommandList::EType>(tmpCommand));
							}
							int tmpIdList; ims.Read(&tmpIdList);
							int posX; int posY; ims.Read(&posX); ims.Read(&posY);
						
							CommandList* tmpCommandList = new CommandList(tmpIdList, tmpCommandListTypes);
							conn->current_commands.push(tmpCommandList);
						}

						std::cout << "Command list size: " << conn->current_commands.size() << std::endl;
						
						break;
					}
				}

				break;

			case Protocol::PTS::JOIN_GAME:
				std::string yConfirm = ims.ReadString();

				std::cout << "Game size: " << games.size() << std::endl;

				// Confirmation
				if (yConfirm == "Y" || yConfirm == "y")
				{
					// Generate new game if there is no game
					if (games.empty()) {
						Game *g = CreateGame(socket->PortReceived());
						PlayerTex* p = g->FindPlayerByPort(socket->PortReceived());
						Send(Protocol::Send(Protocol::STP::JOIN_GAME, 
							p->tex->getPosition().x, p->tex->getPosition().y), p->port);
					}
					else
					{
						for (Game *g : games)
						{
							for (PlayerTex *p : g->players)
							{
								for (Active_Connection *conn : active_con_table)
								{
									if (conn->port != socket->PortReceived())
									{
										if (p->port == conn->port)
										{
											// Join game
											if (PlayerNamesMatch(SearchActiveClientByPort(socket->PortReceived())->name,
												SearchActiveClientByPort(conn->port)->name))
											{
												std::cout << "Joining client to game" << std::endl;
												g->GenPlayers(socket->PortReceived());
												PlayerTex* p = g->FindPlayerByPort(socket->PortReceived());
												Send(Protocol::Send(Protocol::STP::JOIN_GAME,
													p->tex->getPosition().x, p->tex->getPosition().y), p->port);
												
												// Advice other clients

											}
											// Create game
											else
											{
												std::cout << "Creating game" << std::endl;
												Game* g = CreateGame(socket->PortReceived());
												PlayerTex* p = g->FindPlayerByPort(socket->PortReceived());
												Send(Protocol::Send(Protocol::STP::JOIN_GAME,
													p->tex->getPosition().x, p->tex->getPosition().y), p->port);
											}
											break;
										}
									}
								}
							}
						}
					}
				}
				break;
			}


		}
	}

}

void Server::Update()
{
}
