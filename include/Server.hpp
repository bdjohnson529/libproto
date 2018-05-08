#include <iostream>
#include <poll.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#include "Define.h"

#define ACK_LENGTH 100

namespace proto
{
    class Server
    {
        int sockfd, client_fd;  // listen on sock_fd, new connection on client_fd
        int server, client; // socket file descriptors
        int message_length = 500000;	// message length in bytes
        struct pollfd poll_list[3];
        //struct msghdr msg;

    public:
        Server(int portNum);
        std::string Recv();
        std::string RecvAll();
        int Poll();
        int Close();
        int SendAck(int sockfd);
        int RecvAck(int sockfd);
        bool getServerStatus();

    private:
        bool STATUS = false;
    };
}
