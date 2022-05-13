#include "Server.h"

bool Server::IsNewClient(unsigned short _clientID)
{
	for (New_Connection client : new_con_table)
	{
		if (_clientID == client.port) return false; //If It Finds The Client It Means That It Already Existed
	}

	return true;
}

New_Connection* Server::SearchNewClientByPort(unsigned short _clientID)
{
	for (New_Connection client : new_con_table)
	{
		if (_clientID == client.port) return &client;
	}

	return nullptr;
}

New_Connection* Server::SearchNewClientBySalt(unsigned int _clientSalt)
{
	for (New_Connection client : new_con_table)
	{
		unsigned int serSalt = client.clientSALT & client.serverSALT;
		if (_clientSalt = serSalt)
		{
			return &client;
		}
	}
	
	return nullptr;
}

void Server::DeleteNewClients(New_Connection _clientToDelete)
{
	int i = 0;

	while (_clientToDelete.port != new_con_table[i].port)
	{
		i++;
	}

	new_con_table.erase(new_con_table.begin() + i);
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
				New_Connection newClient(socket->PortReceived(), socket->AddressStringReceived(), clientSalt, GenerateSalt(), challenge);
				new_con_table.push_back(newClient);

				std::cout << "send" << std::endl;
				// Send New Challenge Request
				OutputMemoryStream oms;
				oms.Write(Protocol::STP::CHALLENGE_REQUEST);
				oms.Write(challenge);
				oms.Write(newClient.serverSALT); //Send Salt Server
				socket->Send(oms, socket->PortReceived());
			}
			else
			{
				// Search for already existing new client
				New_Connection oldClient = *SearchNewClientByPort(socket->PortReceived());

				// Send Same Challenge Request
				OutputMemoryStream oms;
				oms.Write(Protocol::STP::CHALLENGE_REQUEST);
				oms.Write(oldClient.challenge);
				oms.Write(oldClient.serverSALT); //Send Salt Server
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
			New_Connection myClient = *SearchNewClientBySalt(salt);

			if (myClient.challenge == challengeResponse) // Correct Challenge
			{
				new_con_table.push_back(myClient);

				DeleteNewClients(myClient);

				OutputMemoryStream oms;
				oms.Write(Protocol::STP::HELLO_CLIENT);
				socket->Send(oms, myClient.port);
			}
			else // Fail Challenge
			{
				// We see how many tries has the client
				if (myClient.TRY < 3)
				{
					// Send Same Challenge Request
					OutputMemoryStream oms;
					oms.Write(Protocol::STP::CHALLENGE_REQUEST);
					oms.Write(myClient.challenge);
					oms.Write(myClient.serverSALT); //Send Salt Server
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
