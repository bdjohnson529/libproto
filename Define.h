#pragma once
#include <string>

#define SLEEP 100
#define MAX_MSGS 10 // maximum images to wait on the server

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
