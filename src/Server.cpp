#include "Server.hpp"

using namespace std;

namespace proto
{

    // modified from http://beej.us/guide/bgnet/examples/server.c
    // server seems to time out after a while
    // occasionally cannot restart server
    Server::Server(int portNum)
    {
        struct addrinfo *p;
        socklen_t sin_size;
        struct sigaction sa;
        int rv;

        // use hints struct to return servinfo
        struct addrinfo hints, *servinfo;
        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE; // use my IP

        if ((rv = getaddrinfo(NULL, "5000", &hints, &servinfo)) != 0) {
            fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        }

        // loop through all the results and bind to the first we can
        for(p = servinfo; p != NULL; p = p->ai_next) {
            if ((sockfd = socket(p->ai_family, p->ai_socktype,
                    p->ai_protocol)) == -1) {
                perror("server: socket");
                continue;
            }

            /*
            if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                    sizeof(int)) == -1) {
                perror("setsockopt");
                exit(1);
            }
            */

            if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
                close(sockfd);
                perror("server: bind");
                continue;
            }

            break;
        }

        // Set socket options
        struct timeval timeout;
        timeout.tv_sec = 10;
        timeout.tv_usec = 0;
        int opt1, opt2, opt3, yes=1;
        opt1 = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
        opt2 = setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &yes, sizeof(yes));
        opt3 = setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
        if (opt1 != 0 || opt2 != 0 || opt3 != 0) {
            std::cout << "--------- Error setting socket options : " << strerror(errno) << std::endl;
            close(server);
        }

        freeaddrinfo(servinfo); // all done with this structure

        if (p == NULL)  {
            fprintf(stderr, "server: failed to bind\n");
            exit(1);
        }

        int BACKLOG = 20;
        if (listen(sockfd, BACKLOG) == -1) {
            perror("listen");
            exit(1);
        }

        /*
        sa.sa_handler = sigchld_handler; // reap all dead processes
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = SA_RESTART;
        if (sigaction(SIGCHLD, &sa, NULL) == -1) {
            perror("sigaction");
            exit(1);
        }
        */

        printf("server: waiting for connections...\n");

        struct sockaddr their_addr;
        sin_size = sizeof their_addr;
        client_fd = accept(sockfd, &their_addr, &sin_size);
        if (client_fd == -1) {
            perror("accept");
        }

        // read ipv4 address of client
        struct sockaddr_in *client_addr = (struct sockaddr_in *)&their_addr;
        char s[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(client_addr->sin_addr), s, sizeof s);
        printf("server: got connection from %s\n", s);

        if ( SendAck(client_fd) == -1 )
            return;

        if ( RecvAck(client_fd) == -1 )
            return;

        STATUS = true;


        /*
        // send ack message from new process
        if (!fork()) { // this is the child process
            close(sockfd); // child doesn't need the listener
            if (send(client_fd, buffer, ACK_LENGTH, 0) == -1)
                perror("send");
            close(client_fd);
            exit(0);
        }

            close(client_fd);  // parent doesn't need this
            */

        
    }


    int Server::SendAck(int sockfd)
    {
        std::string ack_message = "ackattack";

        // send ack message to client
        char buffer[ACK_LENGTH];
        memset(&buffer, 0, sizeof buffer);
        std::copy(ack_message.begin(), ack_message.end(), buffer);

        if (send(sockfd, buffer, ACK_LENGTH, 0) == -1){
            perror("send ack");
            return -1;
        }

        return 0;

    }

    int Server::RecvAck(int sockfd)
    {
        std::string ack_message = "ackattack";

        char buffer[ACK_LENGTH];
        memset(&buffer, 0, sizeof buffer);\
        if (recv(sockfd, buffer, ACK_LENGTH, 0) == -1){
            perror("recv ack");
            return -1;
        }

        std::string msg(buffer, buffer + 9);
        if(msg != "ackattack")
            return -1;

        printf("Received ACK from client.\n");

        return 0;

    }


    std::string Server::RecvAll()
    {

        char message[message_length];
        memset(&message, 0, sizeof message);
        int data_size = message_length;

        char * data_ptr = (char*) message;
        int bytes_recv, total_bytes = 0;

        while (data_size > 0)
        {
            //std::cout << "loop 1 " << std::endl;

            //bytes_recv = 0;
            bytes_recv = recv(client_fd, data_ptr, data_size, 0);

            if (bytes_recv <= 0) {
                std::cout << "Incomplete message received." << std::endl;
                return "";
            }

            data_ptr += bytes_recv;
            data_size -= bytes_recv;
            total_bytes += bytes_recv;
        }

        //std::cout << "Total " << total_bytes << " bytes received." << std::endl;

        // Validate checksum
        data_ptr = (char*) message;
        std::string checksum(data_ptr + (message_length - 10), data_ptr + message_length);
        std::string buffer( data_ptr, data_ptr + total_bytes);

        if (total_bytes = message_length && checksum == "jackhammer") {
            std::cout << "Full message received" << std::endl;
            SendAck(client_fd);
            return buffer;
        }
        else {
            std::cout << "Partial message received" << std::endl;
            return "";
        }
    }

    // does not work with long messages
    /*
    std::string Server::Recv()
    {
        char buffer[message_length];

        int bytes_recv = recv(client, buffer, message_length, 0);

        std::cout << "Bytes received: " << bytes_recv << std::endl;

        std::string msg(buffer, buffer + message_length);

        return msg;
    }
    */

    int Server::Poll()
    {
        int retval = poll(poll_list, 1, 10000);
        return retval;
    }

    int Server::Close()
    {
        close(client);
    }

}
