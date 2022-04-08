#include "TcpListener.h"

TcpListener::TcpListener()
{
    if (listener == nullptr)
        listener = new sf::TcpListener();
}

TcpListener::~TcpListener()
{
    if (listener != nullptr)
        delete listener;
}

sf::TcpListener* TcpListener::GetListener()
{
    return listener;
}

Status TcpListener::Listen(Port _port)
{
    Status status(listener->listen(_port.port));

    return status;
}

Status TcpListener::Accept(TcpSocket* _socket)
{
    Status status(listener->accept(*_socket->GetSocket()));

    return status;
}

void TcpListener::Disconnect()
{
    listener->close();
}
