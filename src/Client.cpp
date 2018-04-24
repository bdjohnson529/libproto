#include "Client.hpp"

using namespace std;

namespace proto
{
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

		// set timeout value
		struct timeval timeout;
		timeout.tv_sec = 5;
		timeout.tv_usec = 0;
		int ok = setsockopt(client, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout));
		if(ok < 0)
			cout << "- Error setting socket options..." << endl;
		else
			cout << "- Socket options set..." << endl;

		int success = connect(client, (struct sockaddr *) &server_addr, sizeof(server_addr));

		//-- Verify connection to server
		if (success != 0) {
			cout << "- Error connecting to server. Application terminated" << endl;
			exit(-1);
		} else
			cout << "- Connected to server..." << endl;
	}

	int Client::Send(std::string message)
	{
		int bytes_sent;

		char data[buffer_size];
		std::copy(message.begin(), message.end(), data);

		// checksum to ensure full message receipt
		char * data_ptr = (char *) data;
		char checksum[] = "jackhammer";
		strncpy( (data_ptr + (buffer_size - 10) ), checksum, 10);

		bytes_sent = send(client, &data[0], buffer_size, 0);

		std::cout << bytes_sent << " bytes sent." << endl;
		//std::cout << "size: " << message.size() << "\tmessage substr: " << message.substr(0,100) << std::endl;

		return bytes_sent;
	}

	/***
	int Client::Send(const void * buffer)
	{
		int bytes_sent;

		bytes_sent = send(client, buffer, buffer_size, 0);

		return bytes_sent;
	}
	***/

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


}