#include "ClientID.h"

ClientID::ClientID()
{
}

ClientID::ClientID(unsigned short _port, std::string _address) : port(_port), address(_address)
{
}

ClientID::ClientID(unsigned short _port, std::string _address, unsigned int _saltClient) 
	: port(_port), 
	address(_address), 
	saltClient(_saltClient)
{
}

ClientID::ClientID(unsigned short _port, std::string _address, unsigned int _saltClient, int _challengeRequest)
	: port(_port),
	address(_address),
	saltClient(_saltClient),
	challengeRequest(_challengeRequest)
{
}

ClientID::~ClientID()
{
}