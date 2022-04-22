#pragma once
#include <string>

class Port
{
public:
	std::string ip;
	unsigned short port;

	Port() {};
	Port(unsigned short _port) : port(_port) {};
	Port(std::string _ip, unsigned short _port) : ip(_ip), port(_port) {};
	~Port() {};
};