#pragma once

#include <iostream>

#include <UdpSocket.h>
#include "Constants.h"

class Server
{
	std::vector<unsigned short> myClients;
	UdpSocket* socket;


public:
	Server();
	~Server();

	void Update();
};