#pragma once
#include "Status.h"
#include "Port.h"

#include "InputMemoryStream.h"
#include "OutputMemoryStream.h"
#include "Constants.h"

class UdpSocket
{
	sf::UdpSocket* udpSocket;
	Status statusReceived;
	Port portReceived;
	Address ipReceived;
	
public:
	UdpSocket();
	UdpSocket(sf::UdpSocket* _tcpSocket);
	~UdpSocket();

	sf::UdpSocket* GetSocket();
	void SetSocket(sf::UdpSocket* _udpSocket);
	
	Status Bind(unsigned short _port);
	
	std::string GetPublicIP();
	std::string GetLocalIP();
	unsigned int GetLocalPort();
	
	InputMemoryStream* Receive();
	Status StatusReceived();
	unsigned short PortReceived();
	std::string AddressStringReceived();
	
	Status Send(OutputMemoryStream& _oms, unsigned short _portToSend);
	void Disconnect();
};