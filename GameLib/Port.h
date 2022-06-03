#pragma once
#include <SFML\Network.hpp>
#include <string>

class Port
{
public:
	unsigned short port;

	Port() {};
	Port(unsigned short _port) : port(_port) {};
	~Port() {};
};

class Address
{
public:
	sf::IpAddress ip;

	Address() {};
	Address(sf::IpAddress _ip) : ip(_ip) {};
	~Address() {};

	sf::IpAddress GetAddress() { return ip; };
	std::string GetLocalAddress() { return ip.getLocalAddress().toString(); };
	std::string GetPublicAddress() { return ip.getPublicAddress().toString(); };
};