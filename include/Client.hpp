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
#include <poll.h>

#include <arpa/inet.h>

#include <cstring>

#define TIMEOUT_SEC 4
#define ACK_LENGTH 1000

namespace proto
{
	class Client
	{
		int server_fd, client; // socket file descriptors
		int buffer_size = 500000;
		struct pollfd poll_list[3];
		struct msghdr msg;
		int MAXDATASIZE = 500000;

	public:
		Client(std::string address, std::string port);
		int Send(std::string message);
		int SendAll(const void *data, int data_size);
		int Poll();
		int Close();

		bool STATUS;
	};
}