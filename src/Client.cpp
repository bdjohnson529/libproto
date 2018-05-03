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

		int opt1, opt2, opt3, yes = 1;

		// timeout value
		struct timeval timeout;
		timeout.tv_sec = timeout_sec;
		timeout.tv_usec = 0;

		// set socket options
		opt1 = setsockopt(client, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout));
		opt2 = setsockopt(client, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
		opt3 = setsockopt(client, SOL_SOCKET, SO_REUSEPORT, &yes, sizeof(yes));
		if(opt1 < 0 || opt2 < 0 || opt3 < 0)
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