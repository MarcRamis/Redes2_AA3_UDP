#pragma once
#include <string>

class Port
{
public:
	std::string ip;
	unsigned short port;

	Port() {};
	Port(unsigned short _port) : port(_port) {};
	~Port() {};
};