#pragma once

#include <thread>

#include <UdpSocket.h>
#include "Constants.h"
#include "ClientID.h"

class Client
{
	UdpSocket* socket;
	ClientID id;
	
	// Init
	void WelcomeMessage();
	void GenerateSalt();
	void CombineSalts();
	
	// Receive packets
	void Receive();

public:
	
	Client();
	~Client();
	
	UdpSocket *GetSocket();

	void Update();
};

