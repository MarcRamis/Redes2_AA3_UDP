#pragma once

#include <thread>

#include <UdpSocket.h>
#include "Constants.h"
#include "Tables.h"
#include "Utils.h"
#include "Protocol.h"
#include "Timer.h"

class Client
{
	bool isOpen = true;
	UdpSocket* socket;
	New_Connection* new_con;
	Active_Connection *active_con;
	
	std::string message;
	
	enum class EPhase{ REQUEST_CON, CHALLENGE_RECEIVED, CHAT, GAME};
	EPhase phase = EPhase::REQUEST_CON;

	// Init
	void WelcomeMessage();
	
	// Receive packets
	void Receive();

	// Request Connection 
	void RequestConnection();

public:
	
	Client();
	~Client();
	
	UdpSocket *GetSocket();
	bool GetClientOpen();
	
	void Update();
};

