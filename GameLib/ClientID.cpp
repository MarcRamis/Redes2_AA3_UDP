#include "ClientID.h"

ClientID::ClientID()
{
}

ClientID::ClientID(Port _port, Address _address) : port(_port), address(_address)
{
}

ClientID::~ClientID()
{
}

Port ClientID::GetPort()
{
	return port;
}

Address ClientID::GetAddress()
{
	return address;
}

int ClientID::GetId()
{
	return id;
}

void ClientID::SetSaltClient(int _newSalt)
{
	saltClient = _newSalt;
}

int ClientID::GetSaltClient()
{
	return saltClient;
}

void ClientID::SetSaltServer(int _newSalt)
{
	saltServer = _newSalt;
}

int ClientID::GetSaltServer()
{
	return saltServer;
}

int ClientID::GetTries()
{
	return tries;
}

void ClientID::SetTries(int _tries)
{
	tries = _tries;
}

float ClientID::GetTimeStamp()
{
	return timeStamp;
}

void ClientID::SetTimeStamp(float _timeStamp)
{
	timeStamp = _timeStamp;
}