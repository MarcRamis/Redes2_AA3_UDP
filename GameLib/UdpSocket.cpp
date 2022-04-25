#include "UdpSocket.h"

UdpSocket::UdpSocket()
{
	if (udpSocket == nullptr)
		udpSocket = new sf::UdpSocket();
}

UdpSocket::UdpSocket(sf::UdpSocket* _tcpSocket)
{
}

UdpSocket::~UdpSocket()
{
	if (udpSocket != nullptr)
		delete udpSocket;
}

sf::UdpSocket* UdpSocket::GetSocket()
{
	return udpSocket;
}

void UdpSocket::SetSocket(sf::UdpSocket* _udpSocket)
{
	udpSocket = _udpSocket;
}

Status UdpSocket::Bind(unsigned short _port)
{
	Status status(udpSocket->bind(_port));

	return status;
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

InputMemoryStream* UdpSocket::Receive()
{
	char buffer[BYTE_CAPACITY];
	size_t br;
	Address _sender;
	unsigned short _port;

	statusReceived = udpSocket->receive(buffer, sizeof(buffer), br, _sender.ip, _port);
	portReceived.port = _port;
	ipReceived.ip = _sender.ip;

	InputMemoryStream* ims = new InputMemoryStream(buffer, br);

	return ims;
}

Status UdpSocket::StatusReceived()
{
	return statusReceived;
}

unsigned short UdpSocket::PortReceived()
{
	return portReceived.port;
}

std::string UdpSocket::AddressStringReceived()
{
	return ipReceived.GetAddress().toString();
}

Status UdpSocket::Send(OutputMemoryStream& _oms, unsigned short _portToSend)
{
	Status status(udpSocket->send(_oms.GetBufferPtr(), _oms.GetLength(), GetLocalIP(), _portToSend));
	
	return status;
}

void UdpSocket::Disconnect()
{
	udpSocket->unbind();
}
