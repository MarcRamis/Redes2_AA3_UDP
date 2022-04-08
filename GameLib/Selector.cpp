#include "Selector.h"

Selector::Selector()
{
}

Selector::~Selector()
{
}

void Selector::Add(TcpSocket* _sock)
{
	ss.add(*_sock->GetSocket());
}

void Selector::Add(TcpListener* _listener)
{
	ss.add(*_listener->GetListener());
}

void Selector::Remove(TcpSocket* _sock)
{
	ss.remove(*_sock->GetSocket());
}

void Selector::Remove(TcpListener* _listener)
{
	ss.remove(*_listener->GetListener());
}

void Selector::Clear()
{
	ss.clear();
}

bool Selector::Wait()
{
	return ss.wait();
}

bool Selector::IsReady(TcpSocket* _sock)
{
	return ss.isReady(*_sock->GetSocket());
}

bool Selector::IsReady(TcpListener* _listener)
{
	return ss.isReady(*_listener->GetListener());
}