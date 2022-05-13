#include "Tables.h"

New_Connection::New_Connection()
{
}

New_Connection::New_Connection(unsigned short _port, std::string _address, int _clientSALT, int _serverSALT, int _challenge) 
	: port(_port), address(_address), clientSALT(_clientSALT), serverSALT(_serverSALT), challenge(_challenge)
{
}

New_Connection::New_Connection(unsigned short _port, std::string _address, int _clientSALT)
	: port(_port), address(_address), clientSALT(_clientSALT)
{

}

New_Connection::~New_Connection()
{
}