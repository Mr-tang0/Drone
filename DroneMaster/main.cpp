#include"stdafx.h"
#include "Server.h"
#include "Client.h"

int main(int argc, char* argv[])
{
#ifdef _DEBUG
	std::cout << "Size of Command: " << sizeof(CommandPackage) << std::endl;
#endif // _DEBUG
	std::cout << "Working in Mode: " << argv[1] << std::endl;
	if (strcmp(argv[1], "srv") == 0)
	{
		Server *server = new Server();
		return 0;
	}
	else if (strcmp(argv[1], "cli") == 0)
	{
		char* ip = argv[2];
		if (ip == nullptr)
		{
			std::cout << "Need to provide IP address, like   DroneMaster(.exe) cli 127.0.0.1" << std::endl;
			return -1;
		}
		Client *client = new Client(ip);
		return 0;
	}
	else
	{
		std::cout << "Run in command-line mode with \"DroneMaster(.exe) srv\" for server side or \"DroneMaster(.exe) cli IP\" for client side" << std::endl;
		return 0;
	}
	
}
