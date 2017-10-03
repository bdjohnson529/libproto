#include "Client.hpp"

Client::Client(std::string name, std::string address) : context(1), socket(context, ZMQ_PUB)
{
	socket.setsockopt(ZMQ_IDENTITY, name.c_str(), name.size()); // set client name identifier
    int hwm = PROTO_MAX_MSGS;
    socket.setsockopt(ZMQ_SNDHWM, &hwm, sizeof(hwm)); // maximum number of oustanding messages
	int linger = 0;
	socket.setsockopt(ZMQ_LINGER, &linger, sizeof(linger)); // close socket immediately
    socket.bind(address); // bind publisher to address
}

void Client::Send(std::string msg)
{
	s_send(socket, msg);
}
