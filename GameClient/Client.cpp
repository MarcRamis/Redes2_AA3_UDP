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
				
				std::cout << "CONNECTING TO THE SERVER" << std::endl;;
				//std::cout << " | 'e' to exit" << std::endl;
				std::cout << "Write the next number: " << new_con->challenge << std::endl;
				
				auto future = std::async(std::launch::async, GetLineFromCin);
				//DisconnectFromGetline(future.get());
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
				unsigned short otherClientPort = 0; std::string otherClientMessage;
				ims.Read(&otherClientPort); otherClientMessage = ims.ReadString();
				std::cout << otherClientPort << ": " << otherClientMessage << std::endl;
			}
			break;
			
			case Protocol::STP::HELLO_CLIENT:
			{
				phase = EPhase::CHAT;
				std::string helloMessage = ims.ReadString();
				std::cout << helloMessage << std::endl;
			}
			break;
			case Protocol::STP::DISCONNECT_CLIENT:
				
				std::cout << "You are being disconnected... Bye bye ~~" << std::endl;
				Disconnect();
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
		// Request message every { X } ms
		if (timer.ElapsedSeconds() > REQ_CON_MS)
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

void Client::DisconnectFromGetline(std::string text)
{
	if (text == "e") Disconnect();
}

void Client::Disconnect()
{
	// Advice server
	OutputMemoryStream oms;
	oms.Write(Protocol::PTS::DISCONNECT_CLIENT);
	socket->Send(oms, SERVER_IP);
	
	// Clean memory
	socket->Disconnect();
	if (socket != nullptr) delete socket;
	if (new_con != nullptr) delete new_con;
	if (active_con != nullptr) delete active_con;
	isOpen = false;
}

Client::Client()
{
	socket = new UdpSocket;

	// Init
	WelcomeMessage();

	//Init SFML draw
	//draw = new SFML_Draw();
	//draw->AddSquare(150.0f, 150.0f);
	//std::thread tDraw(Client::draw->UpdateWindow);
	//tDraw.detach();

	// Thread to receive messages
	std::thread tReceive(&Client::Receive, this);
	tReceive.detach();
}

Client::~Client()
{
	Disconnect();
}

void Client::Update()
{
	if (phase == EPhase::CHAT)
	{
		std::cout << "CHAT" << std::endl;
		std::cout << "Write something";
		std::cout << " | 'e' to exit" << std::endl;
		auto future = std::async(std::launch::async, GetLineFromCin);
		message = future.get();

		std::cout << "waiting..." << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		if (message.size() > 0) {

			OutputMemoryStream oms;
			oms.Write(Protocol::PTS::CHAT);
			oms.WriteString(message);
			socket->Send(oms, SERVER_IP);

			DisconnectFromGetline(message);
			message.clear();
		}
	}
}

bool Client::GetClientOpen()
{
	return isOpen;
}
