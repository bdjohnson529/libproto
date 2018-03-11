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
			cout << "\n-Error establishing socket..." << endl;
			exit(-1);
		} else
			cout << "\n- Socket client has been created..." << endl;


		server_addr.sin_family = AF_INET;
		server_addr.sin_port = htons(portNum);

		inet_pton(AF_INET, address.c_str(), &server_addr.sin_addr);

		int success = connect(client, (struct sockaddr *) &server_addr, sizeof(server_addr));

		//-- Verify connection to server
		if (success != 0) {
			cout << "- Error connecting to server. Application terminated" << endl;
			exit(-1);
		} else
			cout << "- Connected to server..." << endl;
	}

	int Client::Send(const void * buffer, int buffer_size)
	{
		//std::string message = "This is a test message.";
		//char buf[28000];
		//std::strcpy(buf, msg.c_str() );
		//cout << buf << endl;

		int bytes_sent;

		bytes_sent = send(client, buffer, buffer_size, 0);

		return bytes_sent;
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
		}

		data_ptr += bytes_sent;
		data_size -= bytes_sent;

		return 1;
	}


}