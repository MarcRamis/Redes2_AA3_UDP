
#include <iostream>
#include <thread>
#include <UdpSocket.h>

void Receive(sf::UdpSocket *socket)
{
	while (true)
	{
		char buffer[1024];
		std::size_t received = 0;
		sf::IpAddress sender;
		unsigned short port;

		sf::Packet pack;
		
		if (socket->receive(pack, sender, port) != sf::Socket::Done)
		{
			std::cout << "No message" << std::endl;
		}
		else
		{
			unsigned short otherClientPort = 0;
			std::string otherClientMessage;
			
			pack >> otherClientPort >> otherClientMessage;
			//InputMemoryStream* ims = new InputMemoryStream(buffer, received);
			//ims->Read(&otherClientPort);
			//otherClientMessage = ims->ReadString();

			std::cout << std::endl;
			std::cout << otherClientPort << ": " << otherClientMessage << std::endl;
		}
	}
}

int main()
{
	bool findPort = false;
	sf::UdpSocket socket;
	unsigned short port = 55002;
	std::string ip = sf::IpAddress::getLocalAddress().toString();
	while (socket.bind(port) != sf::Socket::Status::Done)
	{
		port++;
	}
	
	// Init message
	std::string message = "Mi IP : " + sf::IpAddress::getLocalAddress().toString();
	socket.send(message.c_str(), message.size() + 1, sf::IpAddress::getLocalAddress().toString(), 55001);
	char buffer[1024];
	std::size_t received = 0;
	sf::IpAddress sender;
	unsigned short serverPort;
	if (socket.receive(buffer, sizeof(buffer), received, sender, serverPort) != sf::Socket::Done)
	{
		std::cout << "No message" << std::endl;
	}
	else
	{
		std::cout << sender.toString() << " tells: " << buffer << std::endl;
	}

	// Thread to receive packets
	std::thread tReceive(Receive, &socket);
	tReceive.detach();

	// Send message to server
	std::cout << "Write a message: " << std::endl;
	do
	{
		//OutputMemoryStream oms;
		std::string message = " ";
		
		std::getline(std::cin, message);

		sf::Packet pack;
		pack << message;
		//oms.WriteString(message);
		socket.send(pack, sf::IpAddress::getLocalAddress().toString(), 55001);
		pack.clear();

	} while (true);

	return 0;
}