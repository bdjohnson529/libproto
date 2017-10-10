#include "Server.hpp"

namespace proto
{
	Server::Server(std::string address) : context(1), socket(context, ZMQ_SUB)
	{
		int hwm = PROTO_MAX_MSGS;
	    socket.setsockopt(ZMQ_SNDHWM, &hwm, sizeof(hwm)); // maximum number of oustanding messages
		int linger = 0;
		socket.setsockopt(ZMQ_LINGER, &linger, sizeof(linger)); // close socket immediately
		socket.setsockopt(ZMQ_SUBSCRIBE, "", 0); // subscribe to all messages
		socket.bind(address); // bind server to address
	}

	// blocks and returns the oldest message
	AddressedMessage Server::Recv()
	{
		AddressedMessage msg;
		msg.address = s_recv(socket, false);
		msg.message = s_recv(socket, false);
		return msg;
	}
};
