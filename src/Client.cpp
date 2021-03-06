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
		if ((rv = getaddrinfo(address.c_str(), port.c_str(), &hints, &servinfo)) != 0)
			fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		else if(DEBUG_CLIENT)
			printf("--Successfully got address info.\n");

		// loop through all the results and connect to the first we can
		struct addrinfo *p;
		for(p = servinfo; p != NULL; p = p->ai_next) {
			if(DEBUG_CLIENT)
				printf("--Within client connecting loop.\n");

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
		} else if(DEBUG_CLIENT)
			printf("--Successfully connected to client.\n");

		// read back server address
		char s[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &(((struct sockaddr_in*)p->ai_addr)->sin_addr), s, sizeof s);
		printf("Client: connecting to %s\n", s);

		if ( SetOptions(server_fd) == -1)
			return;

		if ( RecvAck(server_fd) == -1 )
			return;

        if ( SendAck(server_fd) == -1)
        	return;

		this->status = true;

		//freeaddrinfo(servinfo); // all done with this structure

	}


	int Client::Send(std::string message)
	{
		int bytes_sent;

		int message_length = static_cast<int>( message.length() ) + 20;

		char data[message_length];
		memset(&data, 0, sizeof data);

		// header contains the message length
		char buffer_length[10];
		sprintf(buffer_length, "%09d", message_length);
		std::copy( (char *) buffer_length, (char *) buffer_length + 9, (char *) data);

		// message body
		std::copy(message.begin(), message.end(), (char *) data + 9);

		// footer is the checksum
		char * data_ptr = (char *) data;
		char checksum[] = "jackhammer";
		strncpy( (data_ptr + (message_length - 10) ), checksum, 10);

		bytes_sent = send(server_fd, data_ptr, message_length, MSG_NOSIGNAL);

		if(bytes_sent == -1)
			perror("send");

		std::cout << bytes_sent << " bytes sent." << endl;
	}

	int Client::SendAck(int sockfd)
	{
        std::string ack_message = "ackattack";

        // send ack message to server
        char buffer[ACK_LENGTH];
		memset(&buffer, 0, sizeof buffer);
		std::copy(ack_message.begin(), ack_message.end(), buffer);

		int bytes_sent = send(sockfd, buffer, ACK_LENGTH, 0);
		if ( bytes_sent != ACK_LENGTH){
			perror("send ack");
			return -1;
		}
		
		return 0;
	}

	int Client::RecvAck(int sockfd)
	{
		std::cout << "waiting on ack receive" << std::endl;

        std::string ack_message = "ackattack";

		// recv ack from server
        char buffer[ACK_LENGTH];
		memset(&buffer, 0, sizeof buffer);

		if ( Select(sockfd) == 0 )
			return -1;

        if ( (recv(sockfd, buffer, ACK_LENGTH, 0) ) == -1 ) {
        	perror("recv ack");
        	return -1;
        }
	        
        std::string msg(buffer, buffer + 9);
        if (msg != "ackattack")
        	return -1;

        printf("Received ACK from server.\n");

        std::cout << "completed ack receive" << std::endl;
        return 0;
	}

	int Client::Select(int sockfd)
	{
		fd_set set;
		struct timeval timeout;
		FD_ZERO(&set); /* clear the set */
		FD_SET(sockfd, &set); /* add our file descriptor to the set */

		// set the timeout
		timeout.tv_sec = this->rv_timeout;
		timeout.tv_usec = 0;

		int rv = select(sockfd+1, &set, NULL, NULL, &timeout);

		return rv;
	}

	int Client::SetOptions(int sockfd)
	{
		// timeout value
		struct timeval timeout;
		timeout.tv_sec = this->snd_timeout;
		timeout.tv_usec = 0;

		// set socket options
		int opt1, opt2, opt3, opt4, yes=1;
		opt1 = setsockopt(server_fd, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout));
		opt2 = setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
		opt3 = setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &yes, sizeof(yes));
		opt4 = setsockopt(server_fd, SOL_SOCKET, SO_KEEPALIVE, &yes, sizeof(yes));
		if(opt1 < 0 || opt2 < 0 || opt3 < 0 || opt4 < 0){
			cout << "- Error setting socket options..." << endl;
			return -1;
		}

		return 0;
	}

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