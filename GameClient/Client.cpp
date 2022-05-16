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
	
	// Send welcome message to Server (to make server know that this client exists)
	std::thread tRequestCon(&Client::RequestConnection, this);
	tRequestCon.detach();
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
				phase = EPhase::CHALLENGE_RECEIVED;

				ims.Read(&new_con->challenge);
				ims.Read(&new_con->serverSALT); // Receive Salt Server

				// Make challenge
				int challengeAnswer = -1;
				
				std::cout << "CONNECTING TO THE SERVER" << std::endl;
				std::cout << "Write the next number: " << new_con->challenge << std::endl;
				
				auto future = std::async(std::launch::async, GetLineFromCin);
				challengeAnswer = stoi(future.get());

				// Send challenge answer
				OutputMemoryStream oms;
				oms.Write(Protocol::PTS::CHALLENGE_RESPONSE);
				oms.Write(challengeAnswer);
				oms.Write(new_con->clientSALT & new_con->serverSALT); // Send Complete Salt
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
				phase = EPhase::CHAT;
				std::cout << "Connected successfully. " << std::endl;
			}
			break;

			}
		}
	}
}

void Client::RequestConnection()
{
	Timer timer; timer.Start();
	while (phase == EPhase::REQUEST_CON)
	{
		if (timer.ElapsedSeconds() > 0.35f)
		{
			std::string message = "Hi Server, my IP is: ";
			OutputMemoryStream oms;
			oms.Write(Protocol::PTS::HELLO_SERVER); // Header
			oms.WriteString(message); oms.WriteString(new_con->address); // Message
			oms.Write(new_con->clientSALT); // Salt
			oms.Write(new_con->challenge); // Challenge
			socket->Send(oms, SERVER_IP);

			timer.Start();
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
	if (phase == EPhase::CHAT)
	{
		if (message.size() > 0) {

			if (message != "e")
			{
				OutputMemoryStream oms;
				oms.Write(Protocol::PTS::CHAT);
				oms.WriteString(message);
				socket->Send(oms, SERVER_IP);
			}
			else
			{
				isOpen = false;
				// MAKE DISCONNECT
				// ADVICE SERVER
			}
			message.clear();
		}

		std::cout << "CHAT" << std::endl;
		std::cout << "Write something | 'e' to close program" << std::endl;
		auto future = std::async(std::launch::async, GetLineFromCin);
		message = future.get();

		std::cout << "waiting..." << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

bool Client::GetClientOpen()
{
	return isOpen;
}
