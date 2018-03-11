#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include "Define.h"

namespace proto
{
    class Server
    {
        int server, client; // socket file descriptors

    public:
        Server(int portNum);
        std::string Recv(int data_size);
        int RecvAll(void * data, int data_size);
    };
}
