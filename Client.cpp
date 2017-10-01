#include "Client.hpp"

Client::Client(std::string name, std::string address)
{
	context = new zmq::context_t(1); // create zmq network context
	socket = new zmq::socket_t(*context, ZMQ_PUB); // create publisher
	socket->setsockopt(ZMQ_IDENTITY, name.c_str(), name.size()); // set client name identifier
    int sndhwm = MAX_MSGS;
    socket->setsockopt(ZMQ_SNDHWM, &sndhwm, sizeof(sndhwm)); // set high watermark to maximum messages
	int linger = 0;
	socket->setsockopt(ZMQ_LINGER, &linger, sizeof(linger)); // close socket immediately
    socket->bind(PROTOCOL + "://" + address); // bind publisher to address

	// start the client
	thread = std::thread(&Client::do_send, this);
}

Client::Client(std::string name, std::string ip, short port)
{
	Client(name, ip + ":" + std::to_string(port));
}

Client::~Client()
{
	running = false;
	thread.detach();
	delete socket;
	delete context;
}

void Client::Send(std::string msg)
{
	std::cout << "Sending " << msg.size() << " bytes" << std::endl;
	std::lock_guard<std::mutex> guard(lock);
	msgs.push_back(msg);
	while(msgs.size() > MAX_MSGS)
		msgs.pop_front();
}

void Client::do_send()
{
	running = true;
	while(running)
	{
		std::cout << "do_send" << std::endl << std::flush;
		while(msgs.size())
		{
			std::lock_guard<std::mutex> guard(lock);
			s_send(*socket, msgs.front());
			msgs.pop_front();
		}
		s_sleep(SLEEP);
	}
}
