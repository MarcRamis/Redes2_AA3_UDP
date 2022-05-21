#pragma once

#include "UdpSocket.h"

static class Protocol
{
public:
	
	// Client Headers -> Peer to server
	static enum class PTS { HELLO_SERVER, CHALLENGE_RESPONSE, CHAT, DISCONNECT_CLIENT };
	
	// Server Headers -> Server to peer
	static enum class STP { CHALLENGE_REQUEST, HELLO_CLIENT, CHAT, DISCONNECT_CLIENT};

	// Client Functions -> Peer to server
	
	/* Server Functions -> Server to peer | 
	The first three parameters are always the same, then here it comes the data 
	container u want to send */
	static void Send(UdpSocket* socket, STP stp, unsigned short port, std::string str);
	static void Send(UdpSocket* socket, STP stp, unsigned short port, int id, int id2);
	static void Send(UdpSocket* socket, STP stp, unsigned short port, std::string str, unsigned short id);
};