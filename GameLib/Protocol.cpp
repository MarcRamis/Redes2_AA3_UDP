#include "Protocol.h"

void Protocol::Send(UdpSocket* socket, STP protocol, unsigned short port, std::string str)
{
	OutputMemoryStream oms;
	oms.Write(protocol);
	oms.WriteString(str);
	socket->Send(oms, port);
}

void Protocol::Send(UdpSocket* socket, STP protocol, unsigned short port, int id, int id2)
{
	OutputMemoryStream oms;
	oms.Write(protocol);
	oms.Write(id);
	oms.Write(id2);
	socket->Send(oms, port);
}

void Protocol::Send(UdpSocket* socket, STP protocol, unsigned short port, std::string str, unsigned short id)
{
	OutputMemoryStream oms;
	oms.Write(protocol);
	oms.Write(id);
	oms.WriteString(str);
	socket->Send(oms, port);
}
