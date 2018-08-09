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
#define ACK_LENGTH 100
#define DEBUG_CLIENT false

namespace proto
{
	class Client
	{
		int server_fd, client; // socket file descriptors
		int buffer_size = 5000000;
		struct pollfd poll_list[3];
		struct msghdr msg;
		int MAXDATASIZE = 500000;
		int snd_timeout = 4;
		int rv_timeout = 2;

		bool status = false;

	public:
		Client(std::string address, std::string port);
		int Send(std::string message);
		int SendAll(const void *data, int data_size);
		int RecvAck(int sockfd);
		int SendAck(int sockfd);
		int Select(int sockfd);
		int SetOptions(int sockfd);
		int Poll();
		int Close();

		int GetStatus(){return status;};

	};
}