#include "Server.hpp"

Server::Server(std::string address) : context(1), socket(context, ZMQ_SUB)
{
	int hwm = MAX_MSGS;
    socket.setsockopt(ZMQ_SNDHWM, &hwm, sizeof(hwm)); // maximum number of oustanding messages
	int linger = 0;
	socket.setsockopt(ZMQ_LINGER, &linger, sizeof(linger)); // close socket immediately
	socket.setsockopt(ZMQ_SUBSCRIBE, "", 0); // subscribe to all messages
    socket.connect(address); // bind subscriber to address
}

std::vector<AddressedMessage> Server::Recv()
{
	std::vector<AddressedMessage> msgs;
	while(true)
	{
		// get the sender
		std::string sender = s_recv(socket, true);

		// break if empty
		if(sender == "" && msgs.size() > 0)
			return msgs;
		else if(sender == ""  && msgs.size() == 0)
		{
			s_sleep(SLEEP);
			continue;
		}

		// get the message
		std::string message = s_recv(socket, true);

		// break if empty
		if(message == "")
		{
			s_sleep(SLEEP);
			continue;
		}

		msgs.push_back(AddressedMessage(sender,message));
	}
	return msgs;
}
