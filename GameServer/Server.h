#pragma once

#include <UdpSocket.h>
#include "Constants.h"
#include "ClientID.h"
#include "Utils.h"
#include "Protocol.h"

class Server
{
	bool isOpen = true;
	UdpSocket* socket;
	std::vector<ClientID> myNewClients;
	std::vector<ClientID> myClients;
	
	bool IsNewClient(ClientID _clientID);

public:
	Server();
	~Server();

	UdpSocket* GetSocket();
	bool GetServerOpen();

	void Update();
};