#pragma once
#include <mutex>
#include <deque>
#include <thread>
#include <zhelpers.hpp>
#include "Define.h"

// client that sends messages
class Client
{
public:
	Client(std::string name, std::string address);
	Client(std::string name, std::string ip, short port);
	~Client();

	void Send(std::string msg); // queues a message to send

	std::string GetName(){return name;}

private:
	void do_send();

	zmq::context_t *context;
	zmq::socket_t *socket;

	volatile bool running;
	std::mutex lock;
	std::thread thread;
	std::deque<std::string> msgs;
	std::string name;
};
