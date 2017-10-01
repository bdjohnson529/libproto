#include "Server.hpp"

Server::Server(std::string address)
{
	context = new zmq::context_t(1); // create zmq network context
	socket = new zmq::socket_t(*context, ZMQ_SUB); // create publisher
	int linger = 0;
	socket->setsockopt(ZMQ_LINGER, &linger, sizeof(linger)); // close socket immediately
    socket->bind(PROTOCOL + "://" + address); // bind subscriber to address

	// start the server
	thread = std::thread(&Server::do_recv, this);
}

Server::Server(std::string ip, short port)
{
	Server(ip + ":" + std::to_string(port));
}

Server::~Server()
{
	running = false;
	thread.detach();
	delete socket;
	delete context;
}

AddressedMessage Server::Recv()
{
	std::lock_guard<std::mutex> guard(lock);
	if(BLOCKING) // block if enabled
	{
		while(msgs.size() == 0)
			usleep(SLEEP);
	} else if(msgs.size() == 0) {
		return AddressedMessage(); // return empty message if non blocking
	}
	AddressedMessage msg = msgs.front();
	msgs.pop_front();
	return msg;
}

void Server::do_recv()
{
	running = true;
	while(running)
	{
		// get the sender
		std::string sender = s_recv(*socket, true);

		// break if empty
		if(sender == "")
		{
			s_sleep(SLEEP);
			continue;
		}

		//getb the message
		std::string message = s_recv(*socket, true);
		std::cout << "Receiving " << message.size() << " bytes" << std::endl;
		std::lock_guard<std::mutex> guard(lock);
		msgs.push_back(AddressedMessage(sender,message));
		while(msgs.size() > MAX_MSGS)
			msgs.pop_front();
	}
}
