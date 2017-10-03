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
		std::vector<AddressedMessage> Recv(); // waits for at least one message and returns

	private:
		zmq::context_t context;
		zmq::socket_t socket;
	};
};
