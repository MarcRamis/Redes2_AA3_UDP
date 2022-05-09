#include "Server.h"

bool Server::IsNewClient(unsigned short _clientID)
{
	for (ClientID client : myNewClients)
	{
		if (_clientID == client.port) return false; //If It Finds The Client It Means That It Already Existed
	}

	return true;
}

ClientID* Server::SearchNewClientByPort(unsigned short _clientID)
{
	for (ClientID client : myNewClients)
	{
		if (_clientID == client.port) return &client;
	}

	return nullptr;
}

ClientID* Server::SearchNewClientBySalt(unsigned int _clientSalt)
{
	for (ClientID client : myNewClients)
	{
		unsigned int serSalt = client.saltClient & client.saltServer;
		if (_clientSalt = serSalt)
		{
			return &client;
		}
	}
	
	return nullptr;
}

void Server::DeleteNewClients(ClientID _clientToDelete)
{
	int i = 0;

	while (_clientToDelete.port != myNewClients[i].port)
	{
		i++;
	}

	myNewClients.erase(myNewClients.begin() + i);
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
			
			if (IsNewClient(socket->PortReceived()))
			{
				// Create new client
				ClientID newClient(socket->PortReceived(), socket->AddressStringReceived(), clientSalt, GenerateSalt(), challenge);
				myNewClients.push_back(newClient);

				// Send New Challenge Request
				OutputMemoryStream oms;
				oms.Write(Protocol::STP::CHALLENGE_REQUEST);
				oms.Write(challenge);
				oms.Write(newClient.saltServer); //Send Salt Server
				socket->Send(oms, socket->PortReceived());
			}
			else
			{
				// Search for already existing client
				ClientID oldClient = *SearchNewClientByPort(socket->PortReceived());

				// Send Same Challenge Request
				OutputMemoryStream oms;
				oms.Write(Protocol::STP::CHALLENGE_REQUEST);
				oms.Write(oldClient.challengeRequest);
				oms.Write(oldClient.saltServer); //Send Salt Server
				socket->Send(oms, socket->PortReceived());
			}

			
			

			

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
		{
			int challengeResponse;
			unsigned int salt;
			ims.Read(&challengeResponse);
			ims.Read(&salt);
			ClientID myClient = *SearchNewClientBySalt(salt);

			if (myClient.challengeRequest == challengeResponse) // Correct Challenge
			{
				myClients.push_back(myClient);

				DeleteNewClients(myClient);

				OutputMemoryStream oms;
				oms.Write(Protocol::STP::HELLO_CLIENT);
				socket->Send(oms, myClient.port);
			}
			else // Fail Challenge
			{
				// We see how many tries has the client
				if (myClient.tries < 3)
				{
					// Send Same Challenge Request
					OutputMemoryStream oms;
					oms.Write(Protocol::STP::CHALLENGE_REQUEST);
					oms.Write(myClient.challengeRequest);
					oms.Write(myClient.saltServer); //Send Salt Server
					socket->Send(oms, myClient.port);
				}
				else //We Delete The Client
				{
					DeleteNewClients(myClient);
				}


			}
		}
			
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
