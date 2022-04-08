#include "TcpSocket.h"

TcpSocket::TcpSocket()
{
	if (tcpSocket == nullptr)
		tcpSocket = new sf::TcpSocket();
}

TcpSocket::TcpSocket(sf::TcpSocket* _tcpSocket) : tcpSocket(_tcpSocket)
{
}

TcpSocket::~TcpSocket()
{
	if (tcpSocket != nullptr)
		delete tcpSocket;
}

sf::TcpSocket* TcpSocket::GetSocket()
{
	return tcpSocket;
}

void TcpSocket::SetSocket(sf::TcpSocket* _tcpSocket)
{
	tcpSocket = _tcpSocket;
}

std::string TcpSocket::GetRemoteIP()
{
	return tcpSocket->getRemoteAddress().toString();
}

Port TcpSocket::GetRemotePort()
{
	Port port(tcpSocket->getRemotePort());

	return port;
}

Port TcpSocket::GetLocalPort()
{
	Port port(tcpSocket->getLocalPort());

	return port;
}

Status TcpSocket::Connect(std::string _ip, Port _port)
{
	Status status(tcpSocket->connect(_ip, _port.port));
	
	return status;
}

InputMemoryStream* TcpSocket::Receive()
{
	char buffer[10000];
	size_t br;

	messageReceivedStatus = tcpSocket->receive(buffer, 10000, br);
	InputMemoryStream *ims = new InputMemoryStream(buffer, br);
	
	return ims;
}

Status TcpSocket::Send(OutputMemoryStream& _oms)
{	
	Status status(tcpSocket->send(_oms.GetBufferPtr(), _oms.GetLength()));

	return status;
}

Status TcpSocket::StatusReceived() { return messageReceivedStatus; }

void TcpSocket::Disconnect()
{
	tcpSocket->disconnect();
}
