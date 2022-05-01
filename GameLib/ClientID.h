#pragma once

#include "Port.h"
#include "Constants.h"

class ClientID
{
	Port port;
	Address address;
	int id;
	int saltClient, saltServer;
	int tries = 0;
	float timeStamp = TIMESTAMP_DEFAULT;

public:
	
	ClientID();
	ClientID(Port _port, Address _address);
	~ClientID();

	Port GetPort();
	Address GetAddress();
	int GetId();
	void SetSaltClient(int _newSalt);
	int GetSaltClient();
	void SetSaltServer(int _newSalt);
	int GetSaltServer();
	int GetTries();
	void SetTries(int _tries);
	float GetTimeStamp();
	void SetTimeStamp(float _timeStamp);
};