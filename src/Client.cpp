#include "Client.hpp"

using namespace std;

namespace proto
{

	// modified from http://beej.us/guide/bgnet/examples/client.c
	Client::Client(std::string address, string port)
	{

		// set structs to find server address info
		struct addrinfo hints, *servinfo;
		memset(&hints, 0, sizeof hints);
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_PASSIVE;

		// find server address info
		int rv;
		if ((rv = getaddrinfo(address.c_str(), port.c_str(), &hints, &servinfo)) != 0) {
			fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		}

		// loop through all the results and connect to the first we can
		struct addrinfo *p;
		for(p = servinfo; p != NULL; p = p->ai_next) {
			if ((server_fd = socket(p->ai_family, p->ai_socktype,
					p->ai_protocol)) == -1) {
				perror("client: socket");
				continue;
			}

			if (connect(server_fd, p->ai_addr, p->ai_addrlen) == -1) {
				perror("client: connect");
				close(server_fd);
				continue;
			}

			break;
		}

		if (p == NULL) {
			fprintf(stderr, "client: failed to connect\n");
		}


		// timeout value
		struct timeval timeout;
		timeout.tv_sec = 4;
		timeout.tv_usec = 0;

		// set socket options
		int opt1, opt2, opt3, opt4, yes=1;
		opt1 = setsockopt(server_fd, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout));
		opt2 = setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
		opt3 = setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &yes, sizeof(yes));
		opt4 = setsockopt(server_fd, SOL_SOCKET, SO_KEEPALIVE, &yes, sizeof(yes));
		if(opt1 < 0 || opt2 < 0 || opt3 < 0 || opt4 < 0) {
			cout << "- Error setting socket options..." << endl;
			return;
		}

		// read back server address
		char s[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &(((struct sockaddr_in*)p->ai_addr)->sin_addr), s, sizeof s);
		printf("Client: connecting to %s\n", s);

		// recv ack from server
        char buffer[ACK_LENGTH];
		memset(&buffer, 0, sizeof buffer);
        if ( (recv(server_fd, buffer, ACK_LENGTH, 0) ) == -1 ) {
        	perror("recv ack");
        	return;
        }
	        
        std::string msg(buffer, buffer + 9);
        if (msg != "ackattack")
        	return;

        printf("Received ACK from server.\n");

        // send ack message to server
        std::string ack_message = "ackattack";
		memset(&buffer, 0, sizeof buffer);
		std::copy(ack_message.begin(), ack_message.end(), buffer);

		if ( (send(server_fd, buffer, ACK_LENGTH, 0)) == -1){
			perror("send ack");
			return;
		}

		STATUS = true;

		//freeaddrinfo(servinfo); // all done with this structure

	}


	int Client::Send(std::string message)
	{
		int bytes_sent;

		char data[buffer_size];
		memset(&data, 0, sizeof data);
		std::copy(message.begin(), message.end(), data);

		// checksum to ensure full message receipt
		char * data_ptr = (char *) data;
		char checksum[] = "jackhammer";
		strncpy( (data_ptr + (buffer_size - 10) ), checksum, 10);

		std::string check(data_ptr + (buffer_size - 10), data_ptr + buffer_size);
		std::cout << "checksum = " << check << std::endl;
		std::string beginning(data_ptr, data_ptr+10);
		std::cout << "start = " << beginning << std::endl;

		bytes_sent = send(server_fd, data_ptr, buffer_size, MSG_NOSIGNAL);

		if(bytes_sent == -1)
			perror("send");

		std::cout << bytes_sent << " bytes sent." << endl;

		if (bytes_sent == buffer_size)
			return bytes_sent;
		else
			return -1;
	}


	/*
	Client::Client(std::string address, int portNum)
	{

		struct sockaddr_in server_addr;

		server_fd = socket(AF_INET, SOCK_STREAM, 0);

		//-- Verify creation of client
		if (client < 0) {
			cout << "-Error establishing socket..." << endl;
			exit(-1);
		} else
			cout << "- Socket client has been created..." << endl;


		server_addr.sin_family = AF_INET;
		server_addr.sin_port = htons(portNum);

		inet_pton(AF_INET, address.c_str(), &server_addr.sin_addr);

		int opt1, opt2, opt3, opt4, yes = 1;

		SetOptions(server_fd);
		opt4 = setsockopt(client, SOL_SOCKET, SO_KEEPALIVE, &yes, sizeof(yes));
		if(opt1 < 0 || opt2 < 0 || opt3 < 0 || opt4 < 0)

		int success = connect(server_fd, (struct sockaddr *) &server_addr, sizeof(server_addr));

		// set polling list
		poll_list[0].fd = client;
		poll_list[0].events = POLLOUT;

		//-- Verify connection to server
		if (success != 0) {
			close(client);
			cout << "- Error connecting to server." << endl;
			status = false;
		} else {
			cout << "- Connected to server..." << endl;
			status = true;
		}
	}

	void Client::SetTimeout(float seconds, float useconds)
	{

		if( timeout_set == false )
		{
			this->timeout.tv_sec = seconds;
			this->timeout.tv_usec = useconds;
			this->timeout_set = true;
		}

	}

	bool Client::SetOptions(int server_fd)
	{

		int yes = 1;

		if ( setsockopt(client, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout)) )
		{
			perror("setsockopt");
			exit(-1);
		}

		if ( setsockopt(client, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) )
		{
			perror("setsockopt");
			exit(-1);
		}

		if ( setsockopt(client, SOL_SOCKET, SO_REUSEPORT, &yes, sizeof(yes)) )
		{
			perror("setsockopt");
			exit(-1);
		}


	}
	*/

	int Client::Poll()
	{
		int retval = poll(poll_list, 1, 10000);
		return retval;
	}

	int Client::SendAll(const void *data, int data_size)
	{
		const char *data_ptr = (const char*) data;
		int bytes_sent;

		while (data_size > 0)
		{
			bytes_sent = send(client, data_ptr, data_size, 0);

			if (bytes_sent == -1)
				return -1;

			data_ptr += bytes_sent;
			data_size -= bytes_sent;
		}

		return bytes_sent;
	}

	int Client::Close()
	{
		close(client);
	}


}