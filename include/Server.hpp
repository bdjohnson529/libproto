#pragma once
#include <vector>
#include <zhelpers.hpp>
#include "Define.h"

// server that receives messages
namespace proto
{
	class Server
	{
	public:
		Server(std::string address);
		AddressedMessage Recv(); // blocks until a message is available

	private:
		zmq::context_t context;
		zmq::socket_t socket;
	};
};
