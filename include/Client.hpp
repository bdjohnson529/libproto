#pragma once
#include <zhelpers.hpp>
#include "Define.h"

// client that sends messages
namespace proto
{
	class Client
	{
	public:
		Client(std::string name, std::string address);
		void Send(std::string msg);
		std::string GetName(){return name;}

	private:
		zmq::context_t context;
		zmq::socket_t socket;
		std::string name;
	};
};
