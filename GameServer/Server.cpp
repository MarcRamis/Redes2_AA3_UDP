
#include <iostream>
#include <UdpSocket.h>

void ReceiveClients()
{

}

int main()
{
	// Crear Socket y hacer el bind al puerto 55002
	sf::UdpSocket socket;
	socket.bind(55001);
	std::vector<unsigned short> myClients;
	bool isNewClient;

	while (true)
	{
		// Recibir mensaje
		char buffer[1024];
		std::size_t received = 0;
		sf::IpAddress sender;
		unsigned short port;

		sf::Packet pack;
		
		// Receive clients at the beginnng
		if (socket.receive(pack, sender, port) == sf::Socket::Done)
		{
			isNewClient = true;
			for (unsigned short p : myClients)
			{
				if (p == port)
				{
					isNewClient = false;
					break;
				}
			}
			if (isNewClient)
			{
				std::cout << sender.toString() << " tells: " << port << std::endl;

				// Envío de respuesta
				myClients.push_back(port);
				std::string message = "Welcome " + sender.toString();
				socket.send(message.c_str(), message.size() + 1, sender, port);
				
				std::cout << "Table size: " << myClients.size() << std::endl;
			}
			else
			{
				// Read the message
				std::string message = " ";
				pack >> message;
				//InputMemoryStream* ims = new InputMemoryStream(buffer, received);
				//message = ims->ReadString();
				std::cout << port << ": " << message << std::endl;

				sf::Packet packWrite;
				// Send it to the other clients
				for (unsigned short p : myClients)
				{
					if (p != port)
					{
						
						//OutputMemoryStream oms;
						packWrite << port << message;
						//oms.Write(port);
						//oms.WriteString(message);
						socket.send(packWrite, sender, p);
					}
				}
				packWrite.clear();
				pack.clear();
			}
		}
	}

	return 0;
}