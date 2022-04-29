#pragma once

#include <iostream>
#include <thread>

#include <UdpSocket.h>
#include "Constants.h"

class Client
{
	UdpSocket* socket;
	
	void WelcomeMessage();
	void Receive();
	
public:
	
	Client();
	~Client();
	
	UdpSocket *GetSocket();

	void Update();
};

