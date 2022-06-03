#include "Server.h"

int main()
{
	//srand(time(NULL));

	Server server;
	while (server.GetServerOpen())
	{
		server.Update();
	}

	return 0;
}