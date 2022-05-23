#pragma once

#include "Port.h"
#include "Constants.h"
#include "Timer.h"
#include "OutputMemoryStream.h"

struct New_Connection {
	
	unsigned short port;
	std::string address;
	std::string name;
	
	int challenge;
	int clientSALT;
	int serverSALT;
	
	Timer TS; // Almacena el TS del último pkt recibido
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
	
	Timer TS; // Almacena el TS del último pkt recibido

	Active_Connection(unsigned short _port, std::string _address, int _clientSALT, int _serverSALT);
	~Active_Connection();
	Active_Connection();
};

struct Pack_No_Ack {

	unsigned short port; // puerto destino

	int ID; // ID del paquete
	int TS_first; // TS del primer envío del paquete
	int TS_last; // TS del último envío del paquete
	
	OutputMemoryStream content; // Contenido del mensaje
};

struct Pack
{
	int ID; // ID del paquete
	float TS;  // TS de cuando se ha enviado
	OutputMemoryStream *content;
	
	Pack() {};
	Pack(int _ID, float _TS, OutputMemoryStream *_content) : ID(_ID), TS(_TS), content(_content) {};
};