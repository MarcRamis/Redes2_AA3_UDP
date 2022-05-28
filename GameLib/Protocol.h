#pragma once

#include "UdpSocket.h"

static class Protocol
{
public:
	
	// Client Headers -> Client to server
	static enum class PTS { HELLO_SERVER, CHALLENGE_RESPONSE, CHAT, DISCONNECT_CLIENT, COMMAND };
	
	// Server Headers -> Server to peer
	static enum class STP { CHALLENGE_REQUEST, HELLO_CLIENT, CHAT, DISCONNECT_CLIENT, CRI_PACK_RECEIVED, COMMAND};
	
	/* Client Functions -> Client to server |
	Here it comes the data container that the user want to send */
	static OutputMemoryStream *Send(PTS protocol, std::string str, std::string str2, int num);
	static OutputMemoryStream *Send(PTS protocol, int id, int id2);
	static OutputMemoryStream *Send(PTS protocol);
	static OutputMemoryStream* Send(PTS protocol, std::string str);
	
	/* Server Functions -> Server to peer | 
	Here it comes the data container that the user want to send */
	static OutputMemoryStream *Send(STP protocol, std::string str);
	static OutputMemoryStream *Send(STP protocol, int id);
	static OutputMemoryStream *Send(STP protocol);
	static OutputMemoryStream *Send(STP protocol, int id, int id2);
	static OutputMemoryStream *Send(STP protocol, std::string str, unsigned short id);
};