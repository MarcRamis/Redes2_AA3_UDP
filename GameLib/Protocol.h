#pragma once

static class Protocol
{
public:
	
	// Client Headers -> Peer to server
	static enum class PTS { HELLO_SERVER, CHALLENGE_RESPONSE, CHAT };
	
	// Server Headers -> Server to peer
	static enum class STP { CHALLENGE_REQUEST, HELLO_CLIENT, CHAT };
};