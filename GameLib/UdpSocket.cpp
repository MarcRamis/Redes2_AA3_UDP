#include "UdpSocket.h"

UdpSocket::UdpSocket()
{
}

UdpSocket::UdpSocket(sf::UdpSocket* _tcpSocket)
{
}

UdpSocket::~UdpSocket()
{
}

sf::UdpSocket* UdpSocket::GetSocket()
{
	return udpSocket;
}

void UdpSocket::SetSocket(sf::UdpSocket* _udpSocket)
{
	udpSocket = _udpSocket;
}

Status UdpSocket::Bind(unsigned int _port)
{
	return udpSocket->bind(_port);
}

std::string UdpSocket::GetPublicIP()
{
	return sf::IpAddress::getPublicAddress().toString();
}

std::string UdpSocket::GetLocalIP()
{
	return sf::IpAddress::getLocalAddress().toString();
}

unsigned int UdpSocket::GetLocalPort()
{
	return udpSocket->getLocalPort();
}

void UdpSocket::Disconnect()
{
	udpSocket->unbind();
}
