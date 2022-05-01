#include "Client.h"

void Client::WelcomeMessage()
{
	unsigned short port = CLIENT_IP_START;

	// Find for avalaible ports
	while (socket->Bind(port).GetStatus() != Status::EStatusType::DONE)
	{
		port++;
	}

	// Send Welcome Message
	std::string message = "Hi Server, my IP is: ";
	OutputMemoryStream oms;
	oms.WriteString(message); oms.WriteString(socket->GetLocalIP());
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
	while (true)
	{
		InputMemoryStream* ims = socket->Receive();

		if (socket->StatusReceived().GetStatus() != Status::EStatusType::DONE)
		{
			std::cout << "No message" << std::endl;
		}
		else
		{
			unsigned short otherClientPort = 0;
			std::string otherClientMessage;

			ims->Read(&otherClientPort);
			otherClientMessage = ims->ReadString();

			std::cout << otherClientPort << ": " << otherClientMessage << std::endl;
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
