#include <iostream>

#include <Selector.h>
#include "Protocol.h"
#include "Constants.h"
#include "Game.h"
#include "ConsoleControl.h"

#include <vector>
#include <thread>
#include <chrono>
#include <mutex>
#include <list>

std::mutex mtxConexiones;

// Game variables
Game game;
Player player;

void JoinGame(TcpSocket *client)
{
	OutputMemoryStream pack;
	pack.Write(static_cast<int>(Protocol::PEER_BSSProtocol::JOINMATCH));
	pack.WriteString(game.gameName);
	Status status = client->Send(pack);
	if (status.GetStatus() != Status::EStatusType::DONE)
	{
		std::cout << "No se envio el nombre del juego" << std::endl;
	}
}

void AskIfReady(std::vector<TcpSocket*>* _clientes)
{
	std::cout << "Are you ready? 1 - Yes" << std::endl;
	int ackReady;
	do
	{
		std::cin >> ackReady;
		if (ackReady != 1)
		{
			std::cout << "Are you ready? 1 - Yes" << std::endl;
		}
	} while (ackReady != 1);
	
	// Send the packet to every peer to know if is ready
	for (int i = 0; i < _clientes->size(); i++)
	{
		OutputMemoryStream pack;
		pack.Write(static_cast<int>(Protocol::PEER_PEERProtocol::ISREADY));
		Status status = _clientes->at(i)->Send(pack);
		if (status.GetStatus() != Status::EStatusType::DONE)
		{
			std::cout << "El mensaje 'is ready' Peer2Peer no se ha enviado: " << std::endl;
		}
	}
	
	while (!game.gameStart)
	{
		game.gameStart = true;
		for (int i = 0; i < game.gameReady.size(); i++)
		{
			if (!game.gameReady.at(i)) {
				game.gameStart = false;
			}
		}
	}
	game.StartGame(_clientes, player);
}

void ControlServidor(std::vector<TcpSocket*>* _clientes, Selector* _selector, TcpSocket* _socketBSS, bool* _exitBSS, bool * _continueBSS)
{
	// Endless loop that waits for new connections
	while (!(*_exitBSS))
	{
		// Make the selector wait for data on any socket
		InputMemoryStream packet = *_socketBSS->Receive();
		Status status = _socketBSS->StatusReceived();

		if (status.GetStatus() == Status::EStatusType::DONE)
		{
			std::string strRec;
			int tempInt;
			
			//Games List variables
			int sizeGamesList;
			std::string serverName;
			int maxPlayers;
			int currentPlayers;
			bool hasPassWord;
			std::string password;

			//Join Game variables
			int sizeGamesPlayers;
			Port port;
			bool anErrorWithConnection = false;

			packet.Read(&tempInt);

			if (!(*_exitBSS))
			{
				switch (static_cast<Protocol::BSS_PEERProtocol>(tempInt))
				{
				case Protocol::BSS_PEERProtocol::MATCHLIST:

					packet.Read(&sizeGamesList);

					for (int i = 0; i < sizeGamesList; i++)
					{
						serverName = packet.ReadString();
						packet.Read(&maxPlayers);
						packet.Read(&currentPlayers);
						packet.Read(&hasPassWord);
						
						std::cout << "Game name: " << serverName << " Players: " << currentPlayers << " / " << maxPlayers << " Password?: ";
						if (hasPassWord) 
							std::cout << "Yes" << std::endl;
						else
							std::cout << "No" << std::endl;
					}

					*_continueBSS = false;
					break;

				case Protocol::BSS_PEERProtocol::ERROR:

					strRec = packet.ReadString(); // recibo mensaje de errror
					std::cout << strRec << std::endl;
					*_continueBSS = false;
					
					break;
					
				case Protocol::BSS_PEERProtocol::REQ_PWD:

					Protocol::Peer::AckPassword(_socketBSS, game.gameName);
					
					break;

				case Protocol::BSS_PEERProtocol::PEERPLAYERLIST:

					packet.Read(&sizeGamesPlayers);
					packet.Read(&maxPlayers);
					
					player.idTurn = sizeGamesPlayers;
					for (int i = 0; i < sizeGamesPlayers; i++)
					{
						port.ip = packet.ReadString();
						packet.Read(&port.port);

						TcpSocket* tempSock = new TcpSocket();
						status = tempSock->Connect(port.ip, port.port);

						switch (status.GetStatus())
						{
						case Status::EStatusType::DONE:
							
							_clientes->push_back(tempSock);
							game.gameReady.push_back(false);
							_selector->Add(tempSock);
							std::cout << "Se pudo conectar a " << port.ip << " con puerto " << port.port << std::endl;
							
							break;
							
						case Status::EStatusType::DISCONNECTED:
							std::cout << "Se desconecto de " << port.ip << " con puerto " << port.port << std::endl;
							break;
							
						default:
							std::cout << "No se pudo conectar a " << port.ip << " con puerto " << port.port << std::endl;
							anErrorWithConnection = true;
							break;
						}

					}

					if (anErrorWithConnection)
					{
						// Reiniciar sockets y listeners
						*_continueBSS = false;
					}
					else
					{
						_socketBSS->Disconnect();
						delete _socketBSS;
						*_exitBSS = true; //Empezar el chat del main
						
						// Random seed 
						player.randomSeed = _clientes->at(0)->GetRemotePort().port;;
						//std::cout << "My random seed is: " << player.randomSeed << std::endl;
					}

					// Look if the match is full
					packet.Read(&sizeGamesPlayers);
					if (sizeGamesPlayers == maxPlayers)
					{
						OutputMemoryStream pack;
						for (int i = 0; i < _clientes->size(); i++)
						{
							pack.Write(static_cast<int>(Protocol::PEER_PEERProtocol::ACKREADYFORGAME));
							_clientes->at(i)->Send(pack);
						}
						AskIfReady(_clientes);
					}
					break;
				case Protocol::BSS_PEERProtocol::EXITBSSCOM:

					strRec = packet.ReadString();
					std::cout << strRec << std::endl;
					*_exitBSS = true;
					_socketBSS->Disconnect();
					delete _socketBSS;
					
					break;

				case Protocol::BSS_PEERProtocol::NONE:
					break;
				default:
					break;
				}
			}
		}
		else if (status.GetStatus() == Status::EStatusType::DISCONNECTED)
		{
			_socketBSS->Disconnect();
			delete _socketBSS;
			std::cout << "Elimino el socket que se ha desconectado BSS\n";
		}
		else
		{
			std::cout << "Error al recibir de BSS" << std::endl;
		}
	}
}

void ControlPeers(std::vector<TcpSocket*>* _clientes, Selector* _selector, TcpListener* _listener, bool* _exitBSS, bool* _continueBSS)
{
	bool running = true;

	// Create a socket to listen to new connections
	Status status = _listener->Listen(game.localPort);
	if (status.GetStatus() != Status::EStatusType::DONE)
	{
		std::cout << "Error al abrir listener\n";
		exit(0);
	}
	
	_selector->Add(_listener);

	// Endless loop that waits for new connections
	while (running)
	{
		// Make the selector wait for data on any socket
		if (_selector->Wait())
		{
			// Test the listener
			if (_selector->IsReady(_listener))
			{
				// The listener is ready: there is a pending connection
				TcpSocket* client = new TcpSocket;
				if (_listener->Accept(client).GetStatus() == Status::EStatusType::DONE)
				{
					mtxConexiones.lock();

					// Add the new client to the clients list
					std::cout << "Llega el cliente con puerto: " << client->GetRemotePort().port << std::endl;
					_clientes->push_back(client);
					game.gameReady.push_back(false);
					// Add the new client to the selector so that we will
					// be notified when he sends something
					_selector->Add(client);
					
					if (_clientes->size() == 4) 
						_selector->Remove(_listener);

					mtxConexiones.unlock();
				}
				else
				{
					// Error, we won't get a new connection, delete the socket
					std::cout << "Error al recoger conexión nueva\n";
					delete client;
				}
			}
			else
			{
				// The listener socket is not ready, test all other sockets (the clients)
				mtxConexiones.lock();
				for (int it = 0; it < _clientes->size(); ++it)
				{
					TcpSocket* client = _clientes->at(it);
					if (_selector->IsReady(client))
					{
						// The client has sent some data, we can receive it
						InputMemoryStream packet = *client->Receive();

						if (client->StatusReceived().GetStatus() == Status::EStatusType::DONE)
						{
							std::string strRec;
							int tempInt;

							packet.Read(&tempInt);
							
							if (static_cast<Protocol::PEER_PEERProtocol>(tempInt) == Protocol::PEER_PEERProtocol::ACKREADYFORGAME)
							{
								std::thread tReadyToPlay(AskIfReady, _clientes);
								tReadyToPlay.detach();
							}
							else
							{
								switch (static_cast<Protocol::PEER_PEERProtocol>(tempInt))
								{
								case Protocol::PEER_PEERProtocol::SENDMESSAGE:
									
									strRec = packet.ReadString();
									std::cout << client->GetRemotePort().port << ": " << strRec << std::endl;
									break;
								case Protocol::PEER_PEERProtocol::PLAYORGAN:
									
									Protocol::Peer::ReceivedPlayedOrgan(_clientes,packet, player);
									game.NextTurn(_clientes,player);
									
									break;
								case Protocol::PEER_PEERProtocol::INFECTORGAN:
									
									Protocol::Peer::ReceivedOrganInfected(_clientes, packet, player);
									game.NextTurn(_clientes, player);
									break;
								
								case Protocol::PEER_PEERProtocol::DISCARDCARDS:
									Protocol::Peer::ReceivedDiscardCard(_clientes, packet, player);
									game.NextTurn(_clientes, player);
									break;

								case Protocol::PEER_PEERProtocol::YOULOST:

									game.gameEnd = true;
									std::cout << "You lost, bastard..." << std::endl;
									std::cout << "You should take a look at this guy, he plays well ---> https://www.youtube.com/watch?v=7iiE-cE03So" << std::endl;

									std::cout << "Closing program in T-20" << std::endl;
									ConsoleWait(20000.f);
									mtxConexiones.unlock();
									exit(0);

									break;
								case Protocol::PEER_PEERProtocol::VACUNATEORGAN:
									
									Protocol::Peer::ReceivedMedicineCard(_clientes, packet, player);
									game.NextTurn(_clientes, player);

									break;
								case Protocol::PEER_PEERProtocol::LATEXGLOVE:
									
									Protocol::Peer::ReceivedLatexGlove(_clientes, packet, player);
									for (int i = 0; i < _clientes->size() + 1; i++)
									{
										game.NextTurnGlove(_clientes, player);
									}

									ConsoleWait(2000.f);
									game.DrawGame(_clientes, player);
									
									break;

								case Protocol::PEER_PEERProtocol::ORGANTHIEF:
									Protocol::Peer::ReceivedOrganThief(_clientes, packet, player);
									game.NextTurn(_clientes, player);
									break;
									
								case Protocol::PEER_PEERProtocol::ISREADY:
									game.gameReady.at(it) = true;
									std::cout << _clientes->at(it)->GetRemotePort().port << ": Player remote is ready" << std::endl;
									break;

								case Protocol::PEER_PEERProtocol::NONE:
									std::cout << "Error de None" << std::endl;
									break;

								default:
									std::cout << "Error de no encontrar tipo enum" << std::endl;
									break;
								}
							}

							

						}
						else if (client->StatusReceived().GetStatus() == Status::EStatusType::DISCONNECTED)
						{
							game.PlayerDisconnected(_clientes, player, it);

							std::cout << "Player: " << client->GetRemotePort().port << " disconnected" << std::endl;;
							
							_clientes->erase(_clientes->begin() + it);
							_selector->Remove(client);
							client->Disconnect();
							delete client;
							it--;


							if (_clientes->size() == 0)
							{
								std::cout << "The game has finished because all the player have disconnected. " << std::endl;
								std::cout << "Closing program. " << std::endl;
								ConsoleWait(5000.f);
								mtxConexiones.unlock();
								exit(0);
							}
							ConsoleWait(2000.f);
							

							game.DrawGame(_clientes, player);
						}
						else
						{
							std::cout << "Error al recibir de " << client->GetRemotePort().port << std::endl;
						}
					}
				}
				mtxConexiones.unlock();
			}
		}
	}
}

void ConnectToBSS(std::vector<TcpSocket*>* _clientes, Selector* _selector, bool* _exitBSS, bool* _continueBSS)
{
	TcpSocket* sock = new TcpSocket();
	sock->Connect(IP, PORT);

	game.localPort = sock->GetLocalPort().port;

	std::thread messagesServer(ControlServidor, _clientes, _selector, sock, _exitBSS, _continueBSS);
	messagesServer.detach();

	// Parameters to write
	std::string opc;
	int numPlayers = 0;
	bool hasPassword = false;

	do
	{
		if ((*_continueBSS))
		{
			continue;
		}
		std::cout << "1 - Create game" << std::endl 
			<< "2 - Games list" << std::endl 
			<< "3 - Join game" << std::endl;

		std::cin >> opc;
		
		OutputMemoryStream pack;

		if (opc == "1")
		{
			pack.Write(static_cast<int>(Protocol::PEER_BSSProtocol::CREATEMATCH));

			std::cout << "Write game name: " << std::endl;
			std::cin >> opc;
			pack.WriteString(opc);
			
			std::cout << "Write max players (" << MIN_PLAYERS << " - " << MAX_PLAYERS << "):" << std::endl;
			do
			{
				std::cin >> numPlayers;
				if (numPlayers < MIN_PLAYERS || numPlayers > MAX_PLAYERS)
				{
					std::cout << "Write max players (" << MIN_PLAYERS << " - " << MAX_PLAYERS << "):" << std::endl;
				}

			} while (numPlayers < MIN_PLAYERS || numPlayers > MAX_PLAYERS);
			pack.Write(numPlayers);
			
			std::cout << "Password?: 1-Yes 2-No " << std::endl;
			do
			{
				std::cin >> numPlayers;
				if (numPlayers != 1 && numPlayers != 2)
				{
					std::cout << "Password?: 1-Yes 2-No " << std::endl;
				}

			} while (numPlayers != 1 && numPlayers != 2);
			
			if (numPlayers == 1)
			{
				hasPassword = true;
				pack.Write(hasPassword);
				
				std::cout << "Write the password: " << std::endl;
				std::cin >> opc;
				pack.WriteString(opc);
			}
			else
			{
				hasPassword = false;
				pack.Write(hasPassword);
			}

			// Random seed 
			player.randomSeed = game.localPort;
			//std::cout << "My random seed is: " << player.randomSeed << std::endl;
			pack.Write(game.localPort);
			
			player.idTurn = 0;
			
			Status status = sock->Send(pack);
			if (status.GetStatus() != Status::EStatusType::DONE)
			{
				std::cout << "//////////////////// No se ha podido enviar el mensaje con la informacion para crear partida ////////////////////" << std::endl;
			}
		}
		else if (opc == "2")
		{
			pack.Write(static_cast<int>(Protocol::PEER_BSSProtocol::CONSULTMATCHLIST));
			Status status = sock->Send(pack);

			if (status.GetStatus() != Status::EStatusType::DONE)
			{
				std::cout << "//////////////////// No se ha podido enviar el mensaje con la informacion para ver las partidas ////////////////////" << std::endl;
			}
		}
		else if (opc == "3")
		{
			pack.Write(static_cast<int>(Protocol::PEER_BSSProtocol::JOINMATCH));
			
			std::cout << "Write game name to join: " << std::endl;
			std::cin >> game.gameName;
			pack.WriteString(game.gameName);

			Status status = sock->Send(pack);
			if (status.GetStatus() != Status::EStatusType::DONE)
			{
				std::cout << "//////////////////// No se ha podido enviar el nombre de la partida ////////////////////" << std::endl;
			}
		}

		*_continueBSS = true;

	} while (!(*_exitBSS));
	//std::cout << "Salgo del BSS" << std::endl;
}

int main()
{
	std::vector<TcpSocket*> clientes;
	Selector selector;
	TcpListener listener;

	bool exitBSS = false;
	bool continueBSS = false;

	ConnectToBSS(&clientes, &selector, &exitBSS, &continueBSS);
	std::thread tAccepts(ControlPeers, &clientes, &selector, &listener, &exitBSS, &continueBSS);
	tAccepts.detach();

	Protocol::Peer::Chat(&clientes, game.canChat);
	
	return 0;
}