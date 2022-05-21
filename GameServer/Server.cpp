#include "Server.h"

bool Server::IsNewClient(unsigned short _clientID)
{
	for (New_Connection *client : new_con_table)
	{
		if (_clientID == client->port) return false; //If It Finds The Client It Means That It Already Existed
	}

	return true;
}

New_Connection* Server::SearchNewClientByPort(unsigned short _clientID)
{
	for (New_Connection *client : new_con_table)
	{
		if (_clientID == client->port) return client;
	}

	return nullptr;
}

Active_Connection* Server::SearchActiveClientByPort(unsigned short _clientID)
{
	for (Active_Connection* client : active_con_table)
	{
		if (_clientID == client->port) return client;
	}

	return nullptr;
}

New_Connection* Server::SearchNewClientBySalt(unsigned int _clientSalt)
{
	for (New_Connection *client : new_con_table)
	{
		unsigned int serSalt = client->clientSALT & client->serverSALT;
		if (_clientSalt == serSalt)
		{
			return client;
		}
	}
	
	return nullptr;
}

void Server::DeleteNewClients(New_Connection _clientToDelete)
{
	int i = 0;

	while (_clientToDelete.port != new_con_table[i]->port)
	{
		i++;
	}

	new_con_table.erase(new_con_table.begin() + i);
}

void Server::DeleteActiveClients(Active_Connection _clientToDelete)
{
	int i = 0;

	while (_clientToDelete.port != active_con_table[i]->port)
	{
		i++;
	}

	active_con_table.erase(active_con_table.begin() + i);
}

void Server::DisconnectClient()
{
	// Disconnects a new client
	if (!IsNewClient(socket->PortReceived()))
	{
		DeleteNewClients(*SearchNewClientByPort(socket->PortReceived()));
		std::cout << "Deleted the client with port: " << socket->PortReceived() << std::endl;
	}
	// Disconnects an active client
	else
	{
		// Send a message to other clients to let them know a player disconnected
		for (Active_Connection* p : active_con_table)
		{
			if (p->port != socket->PortReceived())
			{
				OutputMemoryStream oms;
				std::string messageDisconnect = "--The player with salt: " +
					std::to_string(SearchActiveClientByPort(socket->PortReceived())->clientSALT
						& SearchActiveClientByPort(socket->PortReceived())->serverSALT) + " has been disconnected--";
				oms.Write(Protocol::STP::CHAT);
				oms.Write(socket->PortReceived());
				oms.WriteString(messageDisconnect);
				socket->Send(oms, p->port);
			}
		}
		
		// Delete if the combined salt coincide with the combined salt client
		DeleteActiveClients(*SearchActiveClientByPort(socket->PortReceived()));
		std::cout << "Deleted the client with port: " << socket->PortReceived() << std::endl;
	}
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
			std::cout << socket->PortReceived() << " tells: " << messageReceived << ipReceived << std::endl;

			// Generate Challenge
			int challenge = GenerateChallenge();
			
			if (IsNewClient(socket->PortReceived()))
			{
				// Create new client
				New_Connection *newClient = new New_Connection(socket->PortReceived(), 
					socket->AddressStringReceived(), clientSalt, GenerateSalt(), challenge);
				new_con_table.push_back(newClient);
				
				// Send New Challenge Request
				OutputMemoryStream oms;
				oms.Write(Protocol::STP::CHALLENGE_REQUEST);
				oms.Write(challenge);
				oms.Write(newClient->serverSALT); //Send Salt Server
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
		}

			break;	

		case Protocol::PTS::CHALLENGE_RESPONSE:
		{
			// Receive challenge answer
			int challengeResponse;
			unsigned int saltServerClient;
			ims.Read(&challengeResponse);
			ims.Read(&saltServerClient);
			
			// Identify client
			New_Connection *myClient = SearchNewClientBySalt(saltServerClient);
			// Challenge answer is correct
			if (myClient->challenge == challengeResponse)
			{
				// Store the new client into active client
				Active_Connection* act_con = new Active_Connection(myClient->port, myClient->address, myClient->clientSALT, myClient->serverSALT);
				active_con_table.push_back(act_con);
				// Delete it from the new clients
				DeleteNewClients(*myClient);
			
				// Send hello message to the client
				OutputMemoryStream oms;
				oms.Write(Protocol::STP::HELLO_CLIENT);
				oms.WriteString("Hello client, welcome to the matrix. ");
				socket->Send(oms, myClient->port); 
			}
			// Challenge answer is wrong
			else
			{
				myClient->TRY--; // Rest try
				std::cout << SearchNewClientBySalt(saltServerClient)->TRY << std::endl;
				// Check Tries
				if (myClient->TRY > 0)
				{
					// Send Same Challenge Request
					OutputMemoryStream oms;
					oms.Write(Protocol::STP::CHALLENGE_REQUEST);
					oms.Write(myClient->challenge);
					oms.Write(myClient->serverSALT); //Send Salt Server
					socket->Send(oms, myClient->port);
				}
				else //We delete the client
				{
					DeleteNewClients(*myClient);
				}
			}
		}
			
			break;

		case Protocol::PTS::CHAT:
		{
			// Read the message
			std::string messageReceived = " ";
			messageReceived = ims.ReadString();
			std::cout << socket->PortReceived() << ": " << messageReceived << std::endl;

			// Send it to other clients
			for (Active_Connection* p : active_con_table)
			{
				if (p->port != socket->PortReceived())
				{
					OutputMemoryStream oms;
					oms.Write(Protocol::STP::CHAT);
					oms.Write(socket->PortReceived());
					oms.WriteString(messageReceived);
					socket->Send(oms, p->port);
				}
			}
		}
			break;
		case Protocol::PTS::DISCONNECT_CLIENT:

			DisconnectClient();
			break;
		}


	}
}
