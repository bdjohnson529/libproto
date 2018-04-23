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

        int yes = 1;
        if(setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
            std::cout << "--------- Error setting socket options" << std::endl;
            exit(-1);
        }

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

        //send(client, "", bufSize, 0);

        std::cout << "--------- Client connected" << std::endl;
    }


    std::string Server::Recv(int data_size)
    {
        char buffer[data_size];

        int bytes_recv = recv(client, buffer, sizeof(buffer), 0);

        std::cout << "Bytes received: " << bytes_recv << std::endl;

        std::string msg = string(&buffer[0], &buffer[data_size - 1] );

        return msg;
    }

    std::string Server::RecvAll()
    {
        char message[message_length];
        int data_size = message_length;

        char * data_ptr = (char*) message;
        int bytes_recv, total_bytes = 0;

        std::string buffer = "";
        while (data_size > 0)
        {
            bytes_recv = recv(client, data_ptr, data_size, 0);

            if (bytes_recv <= 0)
                return buffer;

            //std::cout << "Bytes received: " << bytes_recv << std::endl;

            std::string tmp(data_ptr, data_ptr + bytes_recv);
            buffer.append(tmp);

            data_ptr += bytes_recv;
            data_size -= bytes_recv;
            total_bytes += bytes_recv;
        }

        std::cout << "Total " << total_bytes << " bytes received." << std::endl;

        return buffer;
    }

}
