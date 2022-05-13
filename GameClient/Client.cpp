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
	new_con = new New_Connection(socket->GetLocalPort(), socket->GetLocalIP(), GenerateSalt());
	
	// Send welcome message to Server
	std::string message = "Hi Server, my IP is: ";
	OutputMemoryStream oms;
	oms.Write(Protocol::PTS::HELLO_SERVER); // Header
	oms.WriteString(message); oms.WriteString(new_con->address); // Message
	oms.Write(new_con->clientSALT); // Salt
	oms.Write(new_con->challenge); // Challenge
	socket->Send(oms, SERVER_IP);
	
	// Receive welcome message from Server after passing all anti-hacking tests
	//InputMemoryStream ims = *socket->Receive();
	//std::string messageReceived;
	//messageReceived = ims.ReadString();
	//if (socket->StatusReceived().GetStatus() == Status::EStatusType::DONE)
	//{
	//	std::cout << socket->AddressStringReceived() << " tells: " << messageReceived << std::endl;
	//}
}

void Client::Chat()
{
	// Chat
	std::cout << "Write a message: | 'e' to close program " << std::endl;
	std::string message = " ";

	if (message.size() > 0) {

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
		message.clear();
	}
}

void Client::Receive()
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
			ims.Read(&new_con->challenge);
			ims.Read(&new_con->serverSALT); //Recieve Salt Server

			// Make challenge
			int challengeAnswer = -1;
			std::cout << "Write the next number: " << new_con->challenge << std::endl;
			std::cin >> challengeAnswer;

			// Send challenge answer
			OutputMemoryStream oms;
			int x = static_cast<int>(Protocol::PTS::CHALLENGE_RESPONSE);
			std::cout << x << std::endl;
			oms.Write(Protocol::PTS::CHALLENGE_RESPONSE);
			oms.Write(challengeAnswer);
			oms.Write(new_con->clientSALT & new_con->serverSALT); //Send Complete Salt
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

		case Protocol::STP::HELLO_CLIENT:
		{
			std::cout << "El cliente se conecto correctamente" << std::endl;
		}
		break;

		}
	}
}

Client::Client()
{
	socket = new UdpSocket;

	// Init
	WelcomeMessage();
}

Client::~Client()
{
	delete socket;
}

void Client::Update()
{
	while (isOpen)
	{
		if (message.size() > 0) {

			if (message != "e")
			{
				if (messageState == MessageState::CHAT)
				{
					OutputMemoryStream oms;
					oms.WriteString(message);
					socket->Send(oms, SERVER_IP);
				}
			}
			else
			{
				// MAKE DISCONNECT
				// ADVICE SERVER
			}
			message.clear();
		}
	
		auto future = std::async(std::launch::async, GetLineFromCin);
		message = future.get();
		
		std::cout << "waiting..." << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(1));

		Receive();
	}


	
}

bool Client::GetClientOpen()
{
	return isOpen;
}
