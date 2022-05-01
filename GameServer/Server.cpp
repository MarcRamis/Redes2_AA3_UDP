#include "Server.h"

Server::Server()
{
	socket = new UdpSocket;
	socket->Bind(SERVER_IP);
}

Server::~Server()
{
	delete socket;
}

UdpSocket* Server::GetSocket()
{
	return socket;
}

void Server::Update()
{
	bool isNewClient;

	// Loop
	while (true)
	{
		InputMemoryStream ims = *socket->Receive();
		if (socket->StatusReceived().GetStatus() == Status::EStatusType::DONE)
		{
			// Search if the client exists
			isNewClient = true;
			for (unsigned short p : myClients)
			{
				if (p == socket->PortReceived())
				{
					isNewClient = false;
					break;
				}
			}

			// Receive new clients
			if (isNewClient)
			{
				// Read the message
				std::string messageReceived, ipReceived;
				messageReceived = ims.ReadString(); ipReceived = ims.ReadString();
				std::cout << socket->PortReceived() << " tells: " << messageReceived << ipReceived << std::endl;

				// Send Answer
				std::string messageToSend = "Welcome " + socket->AddressStringReceived();
				OutputMemoryStream oms;
				oms.WriteString(messageToSend);
				socket->Send(oms, socket->PortReceived());

				// Store in table
				myClients.push_back(socket->PortReceived());
				std::cout << "Table size: " << myClients.size() << std::endl;
			}

			// Receive messages from the current clients connected to the server
			else
			{
				// Read the message
				std::string messageReceived = " ";
				messageReceived = ims.ReadString();
				std::cout << socket->PortReceived() << ": " << messageReceived << std::endl;

				// Send it to the other clients
				for (unsigned short p : myClients)
				{
					if (p != socket->PortReceived())
					{
						OutputMemoryStream oms;
						oms.Write(socket->PortReceived());
						oms.WriteString(messageReceived);
						socket->Send(oms, p);
					}
				}
			}
		}
	}
}
