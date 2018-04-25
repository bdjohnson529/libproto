#include "Server.hpp"

using namespace std;

namespace proto
{
    // Starts a server on all available addresses
    Server::Server(int portNum)
    {
        struct sockaddr_in server_addr;
        socklen_t size;

        server = socket(AF_INET, SOCK_STREAM, 0);

        // Initialize server address struct
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = htons(INADDR_ANY);
        server_addr.sin_port = htons(portNum);

        // set timeout
        struct timeval timeout;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;

        int yes = 1;
        if(setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
            std::cout << "--------- Error setting socket options" << std::endl;
            exit(-1);
        }
        if(setsockopt(server, SOL_SOCKET, SO_REUSEPORT, &yes, sizeof(yes)) == -1) {
            std::cout << "--------- Error setting socket options" << std::endl;
            exit(-1);
        }
        /**
        if(setsockopt(server, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) == -1) {
            std::cout << "--------- Error setting socket options" << std::endl;
            exit(-1);
        }**/

        int success = bind(server, (struct sockaddr*) &server_addr, sizeof(server_addr) );
        if(success != 0) {
            std::cout << "--------- Error binding connection\n";
            exit(-1);
        }

        listen(server, 1);

        std::cout << "--------- Waiting for client..." << std::endl;
        client = accept(server, (struct sockaddr *) &server_addr, &size);
        if (client < 0)
            cout << "--------- Error accepting client" << std::endl;

        // Initialize polling
        poll_list[0].fd = client;
        poll_list[0].events = POLLIN;

        std::cout << "--------- Client connected" << std::endl;

        sleep (2);
    }


    std::string Server::Recv()
    {
        char buffer[message_length];

        int bytes_recv = recv(client, buffer, message_length, 0);

        std::cout << "Bytes received: " << bytes_recv << std::endl;

        std::string msg(buffer, buffer + message_length);

        return msg;
    }

    std::string Server::RecvAll()
    {
        char message[message_length];
        int data_size = message_length;

        char * data_ptr = (char*) message;
        int bytes_recv, total_bytes = 0;

        while (data_size > 0)
        {
            bytes_recv = 0;
            bytes_recv = recv(client, data_ptr, data_size, 0);

            if (bytes_recv <= 0) {
                std::cout << "Early terminate" << std::endl;
                return "";
            }

            //std::string tmp(data_ptr, data_ptr + bytes_recv);
            //buffer.append(tmp);

            data_ptr += bytes_recv;
            data_size -= bytes_recv;
            total_bytes += bytes_recv;
        }

        std::cout << "Total " << total_bytes << " bytes received." << std::endl;

        data_ptr = (char*) message;
        std::string checksum(data_ptr + (message_length - 10), data_ptr + message_length);
        std::cout << checksum << std::endl;
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
