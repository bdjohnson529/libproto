#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <time.h>

#include <cstring>

namespace proto
{
	class Client
	{
		int client; // socket file descriptors
		int buffer_size = 3000000;

	public:
		Client(std::string address, int portNum);
		int Send(std::string message);
		int SendAll(const void *data, int data_size);
	};
}