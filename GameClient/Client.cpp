#include "Client.h"

void Client::WelcomeMessage()
{
	unsigned short port = CLIENT_IP_START;

	// Find for avalaible ports
	while (socket->Bind(port).GetStatus() != Status::EStatusType::DONE)
	{
		port++;
	}
	
	// Generate new client
	id = ClientID(socket->GetLocalPort(), socket->GetLocalIP(), GenerateSalt());

	// Send Welcome Message
	std::string message = "Hi Server, my IP is: ";
	OutputMemoryStream oms;
	oms.Write(Protocol::PTS::HELLO_SERVER); // Header
	oms.WriteString(message); oms.WriteString(id.address); // Message
	oms.Write(id.saltClient); // Salt
	oms.Write(id.challengeRequest); // Challenge
	socket->Send(oms, SERVER_IP);

	// Receive Welcome Message
	InputMemoryStream ims = *socket->Receive();
	std::string messageReceived;
	messageReceived = ims.ReadString();
	if (socket->StatusReceived().GetStatus() == Status::EStatusType::DONE)
	{
		std::cout << socket->AddressStringReceived() << " tells: " << messageReceived << std::endl;
	}
}

void Client::Receive()
{
	while (isOpen)
	{
		// Receive message
		InputMemoryStream ims = *socket->Receive();
		if (socket->StatusReceived().GetStatus() == Status::EStatusType::DONE)
		{
			// Receive header
			int _header; ims.Read(&_header);
			
			// Find header
			switch (static_cast<Protocol::STP>(_header))
			{
			case Protocol::STP::CHALLENGE_REQUEST:
			{
				ims.Read(&id.challengeRequest);

				// Make challenge
				int challengeAnswer = -1;
				std::cout << "Write the next number: " << id.challengeRequest << std::endl;
				std::cin >> challengeAnswer;
				
				// Send challenge answer
				OutputMemoryStream oms;
				oms.Write(Protocol::PTS::CHALLENGE_RESPONSE);
				oms.Write(challengeAnswer);
				socket->Send(oms, SERVER_IP);
				
			}
				break;

			case Protocol::STP::CHAT:
			{
				unsigned short otherClientPort = 0;
				std::string otherClientMessage;

				ims.Read(&otherClientPort);
				otherClientMessage = ims.ReadString();

				std::cout << otherClientPort << ": " << otherClientMessage << std::endl;
			}
				break;
			}
		}
	}
}

Client::Client()
{
	socket = new UdpSocket;

	// Init
	WelcomeMessage();
	// Thread to receive messages
	std::thread tReceive(&Client::Receive, this);
	tReceive.detach();
}

Client::~Client()
{
	delete socket;
}

void Client::Update()
{
	// Chat
	std::cout << "Write a message: | 'e' to close program " << std::endl;
	std::string message = " ";
	do
	{
		std::getline(std::cin, message);

		if (message != "e")
		{
			OutputMemoryStream oms;
			oms.WriteString(message);
			socket->Send(oms, SERVER_IP);
		}
		else
		{
			// MAKE DISCONNECT
			// ADVICE SERVER
		}

	} while (message != "e");

}

bool Client::GetClientOpen()
{
	return isOpen;
}
