#pragma once
#include <SFML\Network.hpp>
#include <iostream>

class Status
{
public:
	enum class EStatusType { DONE, DISCONNECTED, PARTIAL, ERROR, NOTREADY, NONE };

private:

	sf::Socket::Status status;

public:
	
	Status() {};
	Status(sf::Socket::Status _status) : status(_status) {};
	~Status() {};

	EStatusType GetStatus()
	{ 
		EStatusType type;
		switch (status)
		{
		case sf::Socket::Done:
			type = EStatusType::DONE;
			//std::cout << "Status type: Done " << std::endl;
			break;
		case sf::Socket::NotReady:
			type = EStatusType::NOTREADY;
			//std::cout << "Status type: NotReady" << std::endl;
			break;
		case sf::Socket::Partial:
			type = EStatusType::PARTIAL;
			//std::cout << "Status type: Partial " << std::endl;
			break;
		case sf::Socket::Disconnected:
			type = EStatusType::DISCONNECTED;
			//std::cout << "Status type: Disconnected" << std::endl;
			break;
		case sf::Socket::Error:
			type = EStatusType::ERROR;
			//std::cout << "Status type: Error " << std::endl;
			break;
		default:
			std::cout << "Error something happened. No status type" << std::endl;
			type = EStatusType::NONE;
			break;
		}
		return type;
	};
};