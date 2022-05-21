#pragma once

#include "Port.h"
#include "Constants.h"

struct New_Connection {
	unsigned short port;
	std::string address;
	std::string name;
	int challenge;
	int clientSALT;
	int serverSALT;
	int TS = TIMESTAMP_DEFAULT; // Almacena el TS del hello
	int TRY = TRIES_DEFAULT;

	New_Connection();
	~New_Connection();
	New_Connection(unsigned short _port, std::string _address, int _clientSALT, int _serverSALT, int _challenge);
	New_Connection(unsigned short _port, std::string _address, int _clientSALT);
};

struct Active_Connection {
	unsigned short port;
	std::string address;
	std::string name;
	int clientSALT;
	int serverSALT;
	int TS = TIMESTAMP_DEFAULT; // Almacena el TS del último pkt recibido
	int PacketID; // ID del último pkt enviado
	int RemoteID; // ID del último pkt enviado
	Active_Connection(unsigned short _port, std::string _address, int _clientSALT, int _serverSALT);
	~Active_Connection();
	Active_Connection();
};

struct Pack_No_Ack{
	
	unsigned short port; // puerto destino
	std::string address; // IP 
	
	int ID; // ID del paquete
	int TS_first = TIMESTAMP_DEFAULT; // TS del primer envío del paquete
	int TS_last = TIMESTAMP_DEFAULT; // TS del último envío del paquete
	std::string message; // Contenido del mensaje
};