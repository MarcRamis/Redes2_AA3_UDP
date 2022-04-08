#pragma once
#include "TcpSocket.h"
#include "TcpListener.h"

class Selector
{
	sf::SocketSelector ss;
public:
	Selector();
	~Selector();
	
	void Add(TcpSocket* _sock);
	void Add(TcpListener* _listener);
	void Remove(TcpSocket* _sock);
	void Remove(TcpListener* _listener);
	void Clear();
	bool Wait();
	bool IsReady(TcpSocket* _sock);
	bool IsReady(TcpListener* _listener);
};


