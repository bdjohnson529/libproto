#include "Server.hpp"

using namespace std;

namespace proto
{

    // modified from http://beej.us/guide/bgnet/examples/server.c
    // server seems to time out after a while
    Server::Server(int portNum)
    {
        int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
        struct addrinfo *p;
        socklen_t sin_size;
        struct sigaction sa;
        int rv;

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

        // main loop to accept clients
        while(1) {

            struct sockaddr their_addr;
            sin_size = sizeof their_addr;
            new_fd = accept(sockfd, &their_addr, &sin_size);
            if (new_fd == -1) {
                perror("accept");
                continue;
            }

            // read ipv4 address of client
            struct sockaddr_in *client_addr = (struct sockaddr_in *)&their_addr;
            char s[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &(client_addr->sin_addr), s, sizeof s);
            printf("server: got connection from %s\n", s);

            if (!fork()) { // this is the child process
                close(sockfd); // child doesn't need the listener
                if (send(new_fd, "Hello, world!", 13, 0) == -1)
                    perror("send");
                close(new_fd);
                exit(0);
            }
            close(new_fd);  // parent doesn't need this
        }
    }


    /*
    // Starts a server on all available addresses
    Server::Server(int portNum)
    {
        bool set = false;

        while (set == false) {

            // Initialize address struct
            struct sockaddr_in server_addr;
            memset( &server_addr, 0, sizeof server_addr );
            server_addr.sin_family = AF_INET;
            server_addr.sin_addr.s_addr = htons(INADDR_ANY);
            server_addr.sin_port = htons(portNum);

            socklen_t size;

            // Create server socket
            server = socket(AF_INET, SOCK_STREAM, 0);

            // Populate recv timeout
            struct timeval timeout;
            timeout.tv_sec = 10;
            timeout.tv_usec = 0;

            // Set socket options
            int yes = 1, opt1, opt2, opt3;
            opt1 = setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
            opt2 = setsockopt(server, SOL_SOCKET, SO_REUSEPORT, &yes, sizeof(yes));
            opt3 = setsockopt(server, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));

            if (opt1 != 0 || opt2 != 0 || opt3 != 0) {
                std::cout << "--------- Error setting socket options : " << strerror(errno) << std::endl;
                close(server);
                continue;
            }

            if ( bind(server, (struct sockaddr *) &server_addr, sizeof(server_addr) ) != 0 ) {
                std::cout << "--------- Error binding connection : " << strerror(errno) << std::endl;
                close(server);
                continue;
            }

            listen(server, 1);

        }

        // Initialize polling
        poll_list[0].fd = client;
        poll_list[0].events = POLLIN;

        sleep (2);
    }
    */

    std::string Server::RecvAll()
    {

        //memset( &msg, 0, sizeof(msg) );
        //this->msg.msg_accrights = (char *)&accept_sd;
        //this->msg.msg_accrightslen = sizeof(accept_sd);

        int client;
        

        /*
        // Loop until there is a connection available
        do {
            std::cout << "--------- Waiting for client..." << std::endl;
            client = accept(server, (struct sockaddr *) &server_addr, &size);
        } while ( client < 0 && errno == 11 );

        if (client < 0) {
            std::cout << "--------- Error accepting client : " << strerror(errno) << std::endl;
            close(server);
        } else {
            std::cout << "---------Connection established." << std::endl;
            set = true;
        }
        */


        char message[message_length];
        int data_size = message_length;

        char * data_ptr = (char*) message;
        int bytes_recv, total_bytes = 0;

        while (data_size > 0)
        {
            bytes_recv = 0;
            bytes_recv = recv(client, data_ptr, data_size, 0);

            if (bytes_recv <= 0) {
                std::cout << "Incomplete message received." << std::endl;
                return "";
            }

            data_ptr += bytes_recv;
            data_size -= bytes_recv;
            total_bytes += bytes_recv;
        }

        std::cout << "Total " << total_bytes << " bytes received." << std::endl;

        // Validate checksum
        data_ptr = (char*) message;
        std::string checksum(data_ptr + (message_length - 10), data_ptr + message_length);
        std::string buffer( data_ptr, data_ptr + total_bytes);

        if (total_bytes = message_length && checksum == "jackhammer") {
            std::cout << "full message received" << std::endl;
            return buffer;
        }
        else {
            std::cout << "partial message received" << std::endl;
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
