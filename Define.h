#pragma once
#include <string>

#define PROTOCOL std::string("tcp")
#define SLEEP 100
#define MAX_MSGS 10
#define BLOCKING 1

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
