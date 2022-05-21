#pragma once

#include <UdpSocket.h>
#include "Constants.h"
#include "Tables.h"
#include "Utils.h"
#include "Protocol.h"

class Server
{
	bool isOpen = true;
	UdpSocket* socket;
	std::vector<New_Connection*> new_con_table;
	std::vector<Active_Connection*> active_con_table;
	
	bool IsNewClient(unsigned short _clientID);
	
	// This search the client port with the server combined port
	New_Connection* SearchNewClientByPort(unsigned short _clientID);
	Active_Connection* SearchActiveClientByPort(unsigned short _clientID);

	// This search the client combined salt with the server combined salt of that client
	New_Connection* SearchNewClientBySalt(unsigned int _clientCombSalt);
	Active_Connection* SearchActiveClientBySalt(unsigned int _clientCombSalt); 

	void DeleteNewClients(New_Connection _clientToDelete);
	void DeleteActiveClients(Active_Connection _clientToDelete);

	void DisconnectClient();

public:
	Server();
	~Server();

	UdpSocket* GetSocket();
	bool GetServerOpen();

	void Update();
};