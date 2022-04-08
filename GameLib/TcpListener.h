#pragma once
#include "TcpSocket.h"

class TcpListener
{
	sf::TcpListener* listener;
public:
	TcpListener();
	~TcpListener();

	sf::TcpListener* GetListener();
	Status Listen(Port _port);
	Status Accept(TcpSocket* _socket);
	void Disconnect();
};

