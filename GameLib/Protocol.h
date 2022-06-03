#pragma once

#include "UdpSocket.h"
#include <queue>

static class Protocol
{
public:
	
	// Client Headers -> Client to server
	static enum class PTS { HELLO_SERVER, CHALLENGE_RESPONSE, CHAT, DISCONNECT_CLIENT, COMMAND, JOIN_GAME, DISCONNECT_FROM_GAME };
	
	// Server Headers -> Server to peer
	static enum class STP { CHALLENGE_REQUEST, HELLO_CLIENT, CHAT, DISCONNECT_CLIENT, CRI_PACK_RECEIVED, COMMAND, JOIN_GAME, UPDATE_VIEW};
	
	/* Client Functions -> Client to server |
	Here it comes the data container that the user want to send */
	static OutputMemoryStream *Send(PTS protocol, std::string str, std::string str2, int num);
	static OutputMemoryStream *Send(PTS protocol, int id, int id2);
	static OutputMemoryStream *Send(PTS protocol, int id, int id2, int id3);
	static OutputMemoryStream *Send(PTS protocol, std::queue<int> id, int id2, int id3, int id4);
	static OutputMemoryStream *Send(PTS protocol);
	static OutputMemoryStream *Send(PTS protocol, std::string str);
	static OutputMemoryStream *Send(PTS protocol, std::string str, std::string str2);
	
	/* Server Functions -> Server to peer | 
	Here it comes the data container that the user want to send */
	static OutputMemoryStream *Send(STP protocol, std::string str);
	static OutputMemoryStream *Send(STP protocol, int id);
	static OutputMemoryStream *Send(STP protocol);
	static OutputMemoryStream *Send(STP protocol, int id, int id2);
	static OutputMemoryStream *Send(STP protocol, float id, float id2);
	static OutputMemoryStream *Send(STP protocol, float id, float id2, unsigned short id3);
	static OutputMemoryStream *Send(STP protocol, std::string str, unsigned short id);
	static OutputMemoryStream *Send(STP protocol, std::string str, std::string str2);
};