
#include <Selector.h>

int main()
{
	// Crear Socket y hacer el bind al puerto 55002
	sf::UdpSocket socket;
	socket.bind(55002);

	while (true)
	{
		// Recibir mensaje
		char buffer[1024];
		std::size_t received = 0;
		sf::IpAddress sender;
		unsigned short port;
		socket.receive(buffer, sizeof(buffer), received, sender, port);
		std::cout << sender.toString() << " dice: " << buffer << std::endl;
		
		// Envío de respuesta
		std::string message = "Bienvenido " + sender.toString();
		socket.send(message.c_str(), message.size() + 1, sender, port);
	}

	return 0;
}