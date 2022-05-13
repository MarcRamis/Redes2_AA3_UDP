#pragma once

#include <thread>

#include <UdpSocket.h>
#include "Constants.h"
#include "Tables.h"
#include "Utils.h"
#include "Protocol.h"

class Client
{
	

	bool isOpen = true;
	UdpSocket* socket;
	New_Connection* new_con;
	Active_Connection *active_con;
	
	enum class MessageState { CHAT, CHALLENGE };
	MessageState messageState = MessageState::CHAT;
	std::string message;

	
	// Init
	void WelcomeMessage();
	
	// Receive packets
	void Receive();

	// Chat
	void SendMessage();

public:
	
	Client();
	~Client();
	
	UdpSocket *GetSocket();
	bool GetClientOpen();
	
	void Update();
};

