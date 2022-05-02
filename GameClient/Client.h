#pragma once

#include <thread>

#include <UdpSocket.h>
#include "Constants.h"
#include "ClientID.h"
#include "Utils.h"
#include "Protocol.h"

class Client
{
	bool isOpen = true;
	UdpSocket* socket;
	ClientID id;
	
	// Init
	void WelcomeMessage();
	
	// Receive packets
	void Receive();

public:
	
	Client();
	~Client();
	
	UdpSocket *GetSocket();
	bool GetClientOpen();

	void Update();
};

