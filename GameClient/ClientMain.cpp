#include "Client.h"

int main()
{
	srand(time(NULL));

	Client client;

	while (client.GetClientOpen())
	{
		client.Update();
	}

	
	return 0;
}