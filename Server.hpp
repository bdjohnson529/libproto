#pragma once
#include <mutex>
#include <deque>
#include <thread>
#include <zhelpers.hpp>
#include "Define.h"

// server that receives messages
class Server
{
public:
	Server(std::string address);
	Server(std::string ip, short port);
	~Server();

	AddressedMessage Recv(); // grabs most recent message, blocks otherwise

private:
	void do_recv();

	zmq::context_t *context;
	zmq::socket_t *socket;

	volatile bool running;
	std::mutex lock;
	std::thread thread;
	std::deque<AddressedMessage> msgs; // messages with sender info
};
