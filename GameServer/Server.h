#pragma once

#include <queue>
#include <mutex>

#include <UdpSocket.h>
#include "Constants.h"
#include "Tables.h"
#include "Protocol.h"
#include "Game.h"
#include "Utils.h"
#include <mutex>

class Server
{
	bool isOpen = true;
	UdpSocket* socket; // Main socket
	std::vector<New_Connection*> new_con_table; // current new connections
	std::vector<Active_Connection*> active_con_table; // current active connections 
	std::vector<Game*> games;

	// Check if is new  client
	bool IsNewClient(unsigned short _clientID);

	//Mutex
	std::mutex tableNewClient;
	std::mutex tableActiveClient;
	std::mutex gameMtx;
	
	// This search the client port with the server combined port
	New_Connection* SearchNewClientByPort(unsigned short _clientID);
	Active_Connection* SearchActiveClientByPort(unsigned short _clientID);

	// This search the client combined salt with the server combined salt of that client
	New_Connection* SearchNewClientBySalt(unsigned int _clientCombSalt);
	Active_Connection* SearchActiveClientBySalt(unsigned int _clientCombSalt); 

	//Send packets
	void Send(OutputMemoryStream *pack, int port);
	void CheckCommands();
	
	// Delete clients
	void DeleteNewClients(New_Connection _clientToDelete);
	void DeleteActiveClients(Active_Connection _clientToDelete);
	void DisconnectClient(int port);
	void DisconnectClientFromGame(int port);

	// Time stamp
	void CheckInactivity();
	void UpdateClientTimer(int port);

	// Update Views
	void UpdateClientView(int _port); // this works only for new clients (i think)
	void UpdateClientView2(int _port);

public:

	Server();
	~Server();

	// Getters
	UdpSocket* GetSocket();
	bool GetServerOpen();

	// Setters

	// Receive Packets
	void Receive();
	
	// Update
	void Update();
	
	Game *CreateGame(int _port);
};