#pragma once
#include <string>

#define PROTO_SLEEP 100
#define PROTO_MAX_MSGS 10 // maximum images to wait on the server
#define PROTO_COMPRESSION true

struct AddressedMessage
{
	std::string address; // sender
	std::string message; // message data

	AddressedMessage()
	{

	}

	AddressedMessage(std::string address, std::string message)
	{
		this->address = address;
		this->message = message;
	}
};
