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

namespace proto
{
    class Server
    {
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
    };
}
