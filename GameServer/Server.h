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
	New_Connection* SearchNewClientByPort(unsigned short _clientID);
	Active_Connection* SearchActiveClientByPort(unsigned short _clientID);
	New_Connection* SearchNewClientBySalt(unsigned int _clientSalt);

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