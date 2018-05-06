#include "Client.hpp"

using namespace std;

namespace proto
{

	// modified from http://beej.us/guide/bgnet/examples/client.c
	Client::Client(std::string address, string port)
	{

		//int sockfd, numbytes;  
		char buf[MAXDATASIZE];
		struct addrinfo *p;
		int rv;

		// used to find server address info
		struct addrinfo hints, *servinfo;
		memset(&hints, 0, sizeof hints);
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_PASSIVE;

		// find server address info
		if ((rv = getaddrinfo(address.c_str(), port.c_str(), &hints, &servinfo)) != 0) {
			fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		}

		// loop through all the results and connect to the first we can
		for(p = servinfo; p != NULL; p = p->ai_next) {
			if ((sockfd = socket(p->ai_family, p->ai_socktype,
					p->ai_protocol)) == -1) {
				perror("client: socket");
				continue;
			}

			if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
				perror("client: connect");
				close(sockfd);
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
		opt1 = setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout));
		opt2 = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
		opt3 = setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &yes, sizeof(yes));
		opt4 = setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &yes, sizeof(yes));
		if(opt1 < 0 || opt2 < 0 || opt3 < 0 || opt4 < 0)
			cout << "- Error setting socket options..." << endl;


		// read back server address
		char s[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &(((struct sockaddr_in*)p->ai_addr)->sin_addr), s, sizeof s);
		printf("client: connecting to %s\n", s);

		char data[500];
		int bytes_sent = send(client, &data[0], buffer_size, MSG_NOSIGNAL);

		freeaddrinfo(servinfo); // all done with this structure

		
		/*
		if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
		    perror("recv");
		    exit(1);
		}

		buf[numbytes] = '\0';

		printf("client: received '%s'\n",buf);
		*/ 

		close(sockfd);
		

	}

	/*
	Client::Client(std::string address, int portNum)
	{

		struct sockaddr_in server_addr;

		client = socket(AF_INET, SOCK_STREAM, 0);

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

		// timeout value
		struct timeval timeout;
		timeout.tv_sec = timeout_sec;
		timeout.tv_usec = 0;

		// set socket options
		opt1 = setsockopt(client, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout));
		opt2 = setsockopt(client, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
		opt3 = setsockopt(client, SOL_SOCKET, SO_REUSEPORT, &yes, sizeof(yes));
		opt4 = setsockopt(client, SOL_SOCKET, SO_KEEPALIVE, &yes, sizeof(yes));
		if(opt1 < 0 || opt2 < 0 || opt3 < 0 || opt4 < 0)
			cout << "- Error setting socket options..." << endl;
		else
			cout << "- Socket options set..." << endl;

		int success = connect(client, (struct sockaddr *) &server_addr, sizeof(server_addr));

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
	*/

	int Client::Send(std::string message)
	{
		int bytes_sent;

		char data[buffer_size];
		std::copy(message.begin(), message.end(), data);

		// checksum to ensure full message receipt
		char * data_ptr = (char *) data;
		char checksum[] = "jackhammer";
		strncpy( (data_ptr + (buffer_size - 10) ), checksum, 10);

		bytes_sent = send(client, &data[0], buffer_size, MSG_NOSIGNAL);

		std::cout << bytes_sent << " bytes sent." << endl;

		if (bytes_sent == buffer_size)
			return bytes_sent;
		else
			return -1;
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