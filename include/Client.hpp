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

#include <cstring>

namespace proto
{
	class Client
	{
		int sockfd, client; // socket file descriptors
		int buffer_size = 500000;
		struct pollfd poll_list[3];
		struct timeval timeout;
		bool timeout_set = false;

	public:
		Client(std::string address, int portNum);
		int Send(std::string message);
		int SendAll(const void *data, int data_size);
		int Poll();
		int Close();
		void SetTimeout(float seconds, float useconds = 0);
		bool SetOptions(int sockfd);

		bool status;
	};
}