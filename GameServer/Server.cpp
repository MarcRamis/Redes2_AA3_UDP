#include "Server.h"

bool Server::IsNewClient(ClientID _clientID)
{
	for (ClientID client : myNewClients)
	{
		if (_clientID.port == client.port) return true;
	}

	return false;
}

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

bool Server::GetServerOpen()
{
	return isOpen;
}

void Server::Update()
{
	// Receive message
	InputMemoryStream ims = *socket->Receive();
	if (socket->StatusReceived().GetStatus() == Status::EStatusType::DONE)
	{
		// Receive header
		int _header; ims.Read(&_header);
		
		// Find Header
		switch (static_cast<Protocol::PTS>(_header))
		{
		case Protocol::PTS::HELLO_SERVER:
		{
			// Read the message
			std::string messageReceived, ipReceived;
			messageReceived = ims.ReadString(); ipReceived = ims.ReadString();
			unsigned int clientSalt; ims.Read(&clientSalt);		
			std::cout << socket->PortReceived() << " tells: " << messageReceived << ipReceived 
				<< "Salt: " << clientSalt << std::endl;

			// Generate Challenge
			int challenge = GenerateChallenge();
						
			// Create new client
			ClientID newClient(socket->PortReceived(), socket->AddressStringReceived(), clientSalt, challenge);
			if (IsNewClient(newClient))
			{
				myNewClients.push_back(newClient);
			}

			// Send Challenge Request
			OutputMemoryStream oms;
			oms.Write(Protocol::STP::CHALLENGE_REQUEST);
			oms.Write(challenge);
			socket->Send(oms, socket->PortReceived());

			//// Send Answer
			//std::string messageToSend = "Welcome " + socket->AddressStringReceived();
			//OutputMemoryStream oms;
			//oms.WriteString(messageToSend);
			//socket->Send(oms, socket->PortReceived());
			//
			//// Store in table
			//ClientID tmpClient(socket->PortReceived(), socket->AddressStringReceived());
			////myClients.push_back(socket->PortReceived());
			//myClients.push_back(tmpClient);
			//std::cout << "Table size: " << myClients.size() << std::endl;
		}

			break;	

		case Protocol::PTS::CHALLENGE_RESPONSE:
			break;

		case Protocol::PTS::CHAT:
			break;
		}

		//// Receive messages from the current clients connected to the server
		//else
		//{
		//	// Read the message
		//	std::string messageReceived = " ";
		//	messageReceived = ims.ReadString();
		//	std::cout << socket->PortReceived() << ": " << messageReceived << std::endl;
		//
		//	// Send it to the other clients
		//	for (unsigned short p : myClients)
		//	{
		//		if (p != socket->PortReceived())
		//		{
		//			OutputMemoryStream oms;
		//			oms.Write(socket->PortReceived());
		//			oms.WriteString(messageReceived);
		//			socket->Send(oms, p);
		//		}
		//	}
		//}
	}
}
