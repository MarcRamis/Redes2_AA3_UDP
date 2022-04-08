
#include <iostream>
#include <Selector.h>

int main()
{
	// Crear socket y hacer bind al puerto 55001
	sf::UdpSocket socket;
	socket.bind(55001);

	while (true)
	{
		// Enviar mensaje al Puerto 55002 en la IP 192.168.1.50
		std::string message = "Mi IP : " + sf::IpAddress::getLocalAddress().toString();
		socket.send(message.c_str(), message.size() + 1, sf::IpAddress::getLocalAddress().toString(), 55002);
		
		// Recepción de respuesta
		char buffer[1024];
		std::size_t received = 0;
		sf::IpAddress sender;
		unsigned short port;
		socket.receive(buffer, sizeof(buffer), received, sender, port);
	}

	return 0;
}