#pragma once
#include "Status.h"
#include "Port.h"
#include "InputMemoryStream.h"
#include "OutputMemoryStream.h"

class UdpSocket
{
	sf::UdpSocket* udpSocket;
	Status messageReceivedStatus;

public:
	UdpSocket();
	UdpSocket(sf::UdpSocket* _tcpSocket);
	~UdpSocket();

	sf::UdpSocket* GetSocket();
	void SetSocket(sf::UdpSocket* _udpSocket);
	Status Bind(unsigned int _port);
	std::string GetPublicIP();
	std::string GetLocalIP();
	unsigned int GetLocalPort();
	Status StatusReceived();
	InputMemoryStream* Receive();
	Status Send(OutputMemoryStream& _oms);

	void Disconnect();
};