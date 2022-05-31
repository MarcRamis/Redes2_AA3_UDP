#include "Client.h"

std::queue<int> convert(std::queue<CommandList::EType> in) {
	
	std::queue<int> out;
	while (!in.empty())
	{
		out.push(static_cast<int>(in.front()));
		in.pop();
	}
	return out;
}

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
	AddCriticPacket(Protocol::Send(
	Protocol::PTS::HELLO_SERVER, "Hi Server, my IP is: ",
		new_con->address, new_con->clientSALT));
}

void Client::Receive()
{
	while (isOpen)
	{
		// Receive message
		InputMemoryStream ims = *socket->Receive();
		if (socket->StatusReceived().GetStatus() == Status::EStatusType::DONE)
		{
			// Update inactivity server timer
			TS->Start();
			
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
				AddCriticPacket(Protocol::Send(
					Protocol::PTS::CHALLENGE_RESPONSE, challengeAnswer, 
					new_con->clientSALT & new_con->serverSALT));
			}
			break;
			
			case Protocol::STP::CHAT:
			{
				unsigned short otherClientPort = 0; std::string otherClientMessage;
				ims.Read(&otherClientPort); otherClientMessage = ims.ReadString();
				std::cout << otherClientPort << ": " << otherClientMessage << std::endl;
			}
			break;

			case Protocol::STP::CRI_PACK_RECEIVED:
			{
				int currentID; ims.Read(&currentID);
				DeleteCriticPacket(currentID);
			}	
			break;
			
			case Protocol::STP::HELLO_CLIENT:
			{
				
				myName = ims.ReadString();
				std::cout << "WELCOME MESSAGE | Hello " + myName + ", welcome to the matrix. " << std::endl;
				
				phase = EPhase::MENU;
			}
			break;
			case Protocol::STP::DISCONNECT_CLIENT:
				
				ConsoleWait(1000.f);
				DisconnectWithoutNotify();
				break;
			case Protocol::STP::JOIN_GAME:
				
				ims.Read(&posX); ims.Read(&posY);
				phase = EPhase::CREATE_GAME;

				break;
				
			case Protocol::STP::UPDATE_VIEW:
				
				unsigned short _receivedPort; ims.Read(&posX); ims.Read(&posY); ims.Read(&_receivedPort);

				// New player
				playerMutex.lock();
				if (player->FindNewPlayer(_receivedPort) == nullptr)
				{
					std::cout << "Adding new player to the game with port: " << _receivedPort << " at location " << posX << " - " << posY << std::endl;
					player->AddNewPlayer(posX, posY, _receivedPort);
				}
				else
				{
					player->SetPlayerPos(sf::Vector2f(posX, posY));
				}
				playerMutex.unlock();

				break;
			}
		}
	}
}

void Client::Send(OutputMemoryStream *pack)
{
	socket->Send(*pack, SERVER_IP);
}

void Client::SendCriticPacket()
{
	Timer timer; timer.Start();
	while (isOpen)
	{
		if (timer.ElapsedSeconds() > T_SEND)
		{
			for (Pack *pack : current_cri_packets)
			{
				pack->content->Write(pack->ID);
				socket->Send(*pack->content, SERVER_IP);
			}
			timer.Start();
		}
	}
}

void Client::SendCommands()
{
	Timer timer; timer.Start();
	while (isOpen)
	{
		if (timer.ElapsedSeconds() > T_SEND_COMMANDS)
		{
			if (!commands_no_validated.empty())
			{
				OutputMemoryStream oms;
				oms.Write(Protocol::PTS::COMMAND);
				oms.Write(static_cast<int>(commands_no_validated.size()));

				for (CommandList* c : commands_no_validated)
				{
					oms.Write(static_cast<int>(c->type.size()));
					std::queue<int> tmpCommandTypes = convert(c->type);
					while (!tmpCommandTypes.empty())
					{
						oms.Write(tmpCommandTypes.front());
						tmpCommandTypes.pop();
					}
					oms.Write(c->id);
					playerMutex.lock();
					oms.Write(player->GetPlayerPos().x);
					oms.Write(player->GetPlayerPos().y);
					playerMutex.unlock();
				}
				
				socket->Send(oms, SERVER_IP);
				
				commands_no_validated.clear();
			}

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
	Send(Protocol::Send(Protocol::PTS::DISCONNECT_CLIENT));
	
	DisconnectWithoutNotify();
}

void Client::DisconnectWithoutNotify()
{
	std::cout << "You are being disconnected for inactivity... Bye bye ~~" << std::endl;

	// Clean memory
	socket->Disconnect();
	if (socket != nullptr) delete socket;
	if (new_con != nullptr) delete new_con;
	
	isOpen = false;
	exit(0);
}

void Client::DeleteCriticPacket(int id)
{
	for (int i = 0; i < current_cri_packets.size(); i++)
	{
		if (current_cri_packets.at(i)->ID == id)
		{
			current_cri_packets.erase(current_cri_packets.begin() + i);
			return;
		}
	}
}

void Client::CreateGame(int posX, int posY)
{
	playerMutex.lock();
	player = new Player(posX, posY);
	player->NewWindow();
	playerMutex.unlock();
}

void Client::AddCriticPacket(OutputMemoryStream *oms)
{
	Pack* tmpPack = new Pack(_tmpIds, 0.5f, oms);
	current_cri_packets.push_back(tmpPack);
	_tmpIds++;
}

void Client::AddCommandList(std::queue<CommandList::EType> commType)
{
	CommandList* command = new CommandList(_tmpCommIds, commType);
	commands_no_validated.push_back(command);
	_tmpCommIds++;
}

void Client::SaveCommands()
{
	Timer timer; timer.Start();
	while (isOpen)
	{
		if (timer.ElapsedSeconds() > T_SAVE_COMMANDS)
		{
			playerMutex.lock();
			if (player != nullptr)
			{
				if (!player->tmp_Commands.empty())
				{
					AddCommandList(player->tmp_Commands);
					player->ClearCommands();
				}
			}
			playerMutex.unlock();
			timer.Start();
		}
	}
}

Client::Client()
{
	std::cout << "STARTING THE CLIENT" << std::endl;

	socket = new UdpSocket;

	// Init
	WelcomeMessage();

	// Thread to receive packages
	std::thread tReceive(&Client::Receive, this);
	tReceive.detach();
	
	// Thread to send packages
	std::thread tSend(&Client::SendCriticPacket, this);
	tSend.detach();
	
	// Thread to send commands
	std::thread tSendCommands(&Client::SendCommands, this);
	tSendCommands.detach();
	
	// Thread to save commands
	std::thread tSaveCommands(&Client::SaveCommands, this);
	tSaveCommands.detach();

	std::thread tCheckInactivity(&Client::CheckInactivity, this);
	tCheckInactivity.detach();
}

Client::~Client()
{
	Disconnect();
}

void Client::Chat()
{
	std::cout << "CHAT";
	std::cout << " | Write something";
	std::cout << " | 'e' to exit" << std::endl;
	auto future = std::async(std::launch::async, GetLineFromCin);
	std::string message = future.get();

	if (message.size() > 0) {

		if (message != "e") Send(Protocol::Send(Protocol::PTS::CHAT, message));
		DisconnectFromGetline(message);
		message.clear();
	}
}

void Client::CheckInactivity()
{
	TS->Start();

	while (isOpen)
	{
		timerInactivityMtx.lock();
		
		if (TS->ElapsedSeconds() > T_INACTIVITY)
		{
			Disconnect();
		}
		timerInactivityMtx.unlock();

		playerMutex.lock();
		if (player != nullptr && player->closedGame)
		{
			Disconnect();
		}
		playerMutex.unlock();
	}

}

void Client::Update()
{
	switch (phase)
	{
	case EPhase::MENU:
	{
		if (!joinGame)
		{
			std::string message;
			std::cout << "JOIN GAME | Type [Y] | 'e' to exit" << std::endl;
			std::cin >> message;

			if (message != "e") Send(Protocol::Send(Protocol::PTS::JOIN_GAME, message));
			DisconnectFromGetline(message);

			joinGame = true;
		}
	}
		break;
	case EPhase::CREATE_GAME:
	{	
		if (!creategame)
		{
			CreateGame(posX, posY);
			creategame = true;
		}
		phase = EPhase::GAME;
	}
		break;

	case EPhase::GAME:
		Chat();
		break;
	}
	
	if (player != nullptr && player->IsWindowActive())
	{
		player->Update();
	}

}

bool Client::GetClientOpen()
{
	return isOpen;
}
