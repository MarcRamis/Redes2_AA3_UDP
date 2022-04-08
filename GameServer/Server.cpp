#include <iostream>

#include "Selector.h"
#include "Match.h"
#include "Protocol.h"
#include "Constants.h"

#include <vector>
#include <map>
#include <thread>
#include <chrono>
#include <mutex>
#include <list>

std::mutex mtxConexiones;

void JoinGame(Match tempGame, std::map<std::string, Match>* games, TcpSocket* client, std::vector <TcpSocket*>* _clientes, Selector &selector, int it)
{
	OutputMemoryStream sendPacket;
	Port tmpPort;
	bool foundGame = false;

	for (auto game = games->begin(); game != games->end(); ++game)
	{
		if (game->second.name == tempGame.name)
		{
			sendPacket.Write(static_cast<int>(Protocol::BSS_PEERProtocol::PEERPLAYERLIST));
			sendPacket.Write(static_cast<int>(game->second.ports.size()));
			sendPacket.Write(static_cast<int>(game->second.maxPlayers));
			
			for (size_t j = 0; j < game->second.ports.size(); j++)
			{
				sendPacket.WriteString(game->second.ports.at(j).ip);
				sendPacket.Write(game->second.ports.at(j).port);
			}
			tmpPort.ip = client->GetRemoteIP();
			tmpPort.port = client->GetRemotePort().port;
			game->second.ports.push_back(tmpPort);
			foundGame = true;

			sendPacket.Write(static_cast<int>(game->second.ports.size()));
			client->Send(sendPacket);

			if (game->second.ports.size() >= game->second.maxPlayers)
			{
				games->erase(game);
			}
			break;
		}
	}
	if (!foundGame)
	{
		std::string errorTxt = "Game could't be found";

		sendPacket.Write(static_cast<int>(Protocol::BSS_PEERProtocol::ERROR));
		sendPacket.WriteString(errorTxt);
		client->Send(sendPacket);
	}
	else
	{
		_clientes->erase(_clientes->begin() + it);
		selector.Remove(client);
		client->Disconnect();
		delete client;
		it--;
	}

}

void ControlServidor(std::vector<TcpSocket*>* _clientes, std::map<std::string,Match>* games)
{
	bool running = true;

	TcpListener listener;
	Status status = listener.Listen(PORT);
	if (status.GetStatus() != Status::EStatusType::DONE)
	{
		std::cout << "Error al abrir listener\n";
		exit(0);
	}

	Selector selector;
	selector.Add(&listener);
	
	while (true)
	{
		std::cout << "Waiting connection" << std::endl;
		if (selector.Wait())
		{
			if (selector.IsReady(&listener))
			{
				TcpSocket* client = new TcpSocket;
				if (listener.Accept(client).GetStatus() == Status::EStatusType::DONE)
				{
					std::cout << "Llega el cliente con puerto: " << client->GetRemotePort().port << std::endl;
					_clientes->push_back(client);
					selector.Add(client);
				}
				else
				{
					std::cout << "Error al recoger conexión nueva\n";
					delete client;
				}
			}
			else
			{
				mtxConexiones.lock();
				for (size_t it = 0; it < _clientes->size(); ++it)
				{
					TcpSocket* client = _clientes->at(it);
					if (selector.IsReady(client))
					{
						// The client has sent some data, we can receive it
						InputMemoryStream packet = *client->Receive();
						if (client->StatusReceived().GetStatus() == Status::EStatusType::DONE)
						{
							std::cout << "Recibido mensaje de Peer" << std::endl;

							int type;
							packet.Read(&type);
							
							Match tempGame;
							Port tmpPort;

							bool nameRepeats = false;
							bool foundGame = false;
							
							OutputMemoryStream sendPacket;
							switch (static_cast<Protocol::PEER_BSSProtocol>(type))
							{
							case Protocol::PEER_BSSProtocol::CREATEMATCH:
								
								std::cout << "Creating game" << std::endl;

								tempGame.name = packet.ReadString(); // Save game name
								
								for (auto game = games->begin(); game != games->end(); ++game)
								{
									if (game->second.name == tempGame.name)
									{
										std::string textError = "Game name exists";
										sendPacket.Write(static_cast<int>(Protocol::BSS_PEERProtocol::ERROR));
										sendPacket.WriteString(textError);
										client->Send(sendPacket);
										nameRepeats = true;
										
										break;
									}
								}
								if (!nameRepeats)
								{
									// SET PARAMETERS 
									packet.Read(&tempGame.maxPlayers);
									tmpPort.ip = client->GetRemoteIP();
									tmpPort.port = client->GetRemotePort().port;
									tempGame.ports.push_back(tmpPort);
									
									packet.Read(&tempGame.hasPassword);
									if (tempGame.hasPassword)
									{
										tempGame.pw = packet.ReadString();
									}
									
									std::string textDebug = "The game was created";
									std::cout << textDebug << std::endl;
									games->insert(std::pair<std::string,Match>(tempGame.name, tempGame));

									sendPacket.Write(static_cast<int>(Protocol::BSS_PEERProtocol::EXITBSSCOM));
									sendPacket.WriteString(textDebug);
									client->Send(sendPacket);
									
									//Cerrar comunicacion con este socket
									_clientes->erase(_clientes->begin() + it);
									selector.Remove(client);
									client->Disconnect();
									delete client;
									it--;
								}

								break;
								
							case Protocol::PEER_BSSProtocol::CONSULTMATCHLIST:
								
								sendPacket.Write(static_cast<int>(Protocol::BSS_PEERProtocol::MATCHLIST));
								sendPacket.Write(static_cast<int>(games->size()));
								for (auto game = games->begin(); game != games->end(); ++game)
								{
									sendPacket.WriteString(game->second.name);
									sendPacket.Write(game->second.maxPlayers);
									sendPacket.Write(static_cast<int>(game->second.ports.size()));
									sendPacket.Write(game->second.hasPassword);
								}
								client->Send(sendPacket);
								break;
								
							case Protocol::PEER_BSSProtocol::JOINMATCH:
								
								tempGame.name = packet.ReadString();
								for (auto game = games->begin(); game != games->end(); ++game)
								{
									if (game->second.name == tempGame.name)
									{
										// To make the current client to join match know if there is password										
										if (game->second.hasPassword)
										{
											sendPacket.Write(static_cast<int>(Protocol::BSS_PEERProtocol::REQ_PWD));
											client->Send(sendPacket);
										}
										else
										{
											JoinGame(tempGame, games, client, _clientes, selector, it);
										}
										break;
									}
								}
								break;

							case Protocol::PEER_BSSProtocol::ACK_PWD:

								tempGame.name = packet.ReadString();
								tempGame.pw = packet.ReadString();
								for (auto game = games->begin(); game != games->end(); ++game)
								{
									if (game->second.name == tempGame.name)
									{
										// To make the current client to join match know if there is password										
										if (game->second.pw != tempGame.pw)
										{
											sendPacket.Write(static_cast<int>(Protocol::BSS_PEERProtocol::REQ_PWD));
											client->Send(sendPacket);
										}
										else
										{
											JoinGame(tempGame, games, client, _clientes, selector, it);
										}
										break;
									}
								}

								break;
								
							case Protocol::PEER_BSSProtocol::NONE:
								std::cout << "Error de tipo de mensaje por NONE" << std::endl;
								break;
							default:
								std::cout << "Error de tipo de mensaje no lo pudo identificar" << std::endl;
								break;
							}

						}
						else if (client->StatusReceived().GetStatus() == Status::EStatusType::DISCONNECTED)
						{
							_clientes->erase(_clientes->begin() + it);
							selector.Remove(client);
							client->Disconnect();
							delete client;
							it--;

							std::cout << "Elimino el socket que se ha desconectado\n";
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

	listener.Disconnect();
}

int main()
{
	std::vector<Port*> ids;
	std::vector<TcpSocket*> clientes;
	std::map<std::string, Match> games;
	
	ControlServidor(&clientes, &games);

	return 0;
}