#pragma once

#include <UdpSocket.h>
#include "Constants.h"
#include "ClientID.h"

class Server
{
	UdpSocket* socket;
	std::vector<ClientID> myNewClients;
	std::vector<ClientID> myClients;

	void GenerateSalt();
	void CombineSalts();
	
public:
	Server();
	~Server();

	UdpSocket* GetSocket();

	void Update();
};