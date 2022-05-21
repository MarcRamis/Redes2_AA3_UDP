#include "Protocol.h"

void Protocol::Send(UdpSocket* socket, STP stp, unsigned short port, std::string str)
{
	OutputMemoryStream oms;
	oms.Write(stp);
	oms.WriteString(str);
	socket->Send(oms, port);
}

void Protocol::Send(UdpSocket* socket, STP stp, unsigned short port, int id, int id2)
{
	OutputMemoryStream oms;
	oms.Write(stp);
	oms.Write(id);
	oms.Write(id2);
	socket->Send(oms, port);
}

void Protocol::Send(UdpSocket* socket, STP stp, unsigned short port, std::string str, unsigned short id)
{
	OutputMemoryStream oms;
	oms.Write(stp);
	oms.Write(id);
	oms.WriteString(str);
	socket->Send(oms, port);
}
