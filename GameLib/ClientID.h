#pragma once

#include "Port.h"
#include "Constants.h"

struct ClientID
{
	unsigned short port;
	std::string address;
	std::string name;
	
	int challengeRequest;
	unsigned int saltClient, saltServer;
	int tries = TRIES_DEFAULT;
	float timeStamp = TIMESTAMP_DEFAULT;
	
	ClientID();
	ClientID(unsigned short _port, std::string _address);
	ClientID(unsigned short _port, std::string _address, unsigned int _saltClient);
	ClientID(unsigned short _port, std::string _address, unsigned int _saltClient, int _challengeRequest);
	ClientID(unsigned short _port, std::string _address, unsigned int _saltClient, unsigned int _saltServer, int _challengeRequest);
	~ClientID();
};