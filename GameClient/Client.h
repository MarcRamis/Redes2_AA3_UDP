#pragma once

#include <thread>

#include <UdpSocket.h>
#include "Constants.h"
#include "Tables.h"
#include "Protocol.h"
#include "Player.h"
#include "ConsoleControl.h"

class Client
{
	enum class EPhase{ REQUEST_CON, CHALLENGE_RECEIVED, CHAT, GAME};
	EPhase phase = EPhase::REQUEST_CON;
	bool isOpen = true;
	UdpSocket* socket;
	
	// This has to change for the info that our client needs
	New_Connection* new_con;
	Timer TS; // Time stamp from server
	
	// Critic packets to send
	std::vector<Pack*> current_cri_packets;
	int _tmpIds = 0;
	std::vector<Command*> commands_no_validated;
	int _tmpCommIds = 0;

	// Player
	Player *player = new Player(150, 100 , 100.0f);

	// Init
	void WelcomeMessage();
	
	// Receive packets
	void Receive();

	// Send packets
	void Send(OutputMemoryStream *pack);
	void SendCriticPacket();
	void SendCommands();
	
	// Disconnect
	void DisconnectFromGetline(std::string text);
	void Disconnect();
	void DisconnectWithoutNotify();

	// Add critic packets
	void AddCriticPacket(OutputMemoryStream *oms);
	void AddCommandList(std::queue<Command::EType> commType);
	
	// Save commands
	void SaveCommands();

	// Delete packets
	void DeleteCriticPacket(int id);

public:
	
	Client();
	~Client();
	
	UdpSocket *GetSocket();
	bool GetClientOpen();
	
	void Update();
};

