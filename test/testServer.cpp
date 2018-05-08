#include "Server.hpp"

int main()
{

	proto::Server server(5000);

	while(1)
	{
		std::string message = server.RecvAll();


		if ( message.empty() )
			sleep(1);
	}

	return 0;
}