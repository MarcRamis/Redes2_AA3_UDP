
#include <iostream>
#include <Selector.h>

int main()
{
	bool findPort = false;
	// Crear socket y hacer bind al puerto 55001
	sf::UdpSocket socket;
	unsigned short port = 55001;

	while (socket.bind(port) != sf::Socket::Status::Done)
	{
		port++;
	}
	

	while (true)
	{
		// Enviar mensaje al Puerto 55002 en la IP 192.168.1.50
		int x = 1;
		OutputMemoryStream oms;
		oms.Write(x);
		//std::string message = "Mi IP : " + sf::IpAddress::getLocalAddress().toString();
		socket.send(oms.GetBufferPtr(), oms.GetLength(), sf::IpAddress::getLocalAddress().toString(), 55002);
		
		// Recepción de respuesta
		char buffer[1024];
		std::size_t received = 0;
		sf::IpAddress sender;
		unsigned short port;
		socket.receive(buffer, sizeof(buffer), received, sender, port);
		std::cout << sender.toString() << " dice: " << buffer << std::endl;
	}

	return 0;
}