#pragma once
#include "Status.h"
#include "Port.h"
#include "InputMemoryStream.h"
#include "OutputMemoryStream.h"

class TcpSocket
{
	sf::TcpSocket* tcpSocket;
	Status messageReceivedStatus;
	
public:
	TcpSocket();
	TcpSocket(sf::TcpSocket* _tcpSocket);
	~TcpSocket();

	sf::TcpSocket* GetSocket();
	void SetSocket(sf::TcpSocket *_tcpSocket);
	std::string GetRemoteIP();
	Port GetRemotePort();
	Port GetLocalPort();
	Status Connect(std::string _ip, Port _port);
	Status StatusReceived();
	InputMemoryStream* Receive();
	Status Send(OutputMemoryStream& _oms);
	
	void Disconnect();
};
