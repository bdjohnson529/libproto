#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include "Compress.hpp"

enum MessageType
{
	IMAGE
};

// provides an abstract message class to serialize different message types
class Message
{
	struct MessageHeader
	{
		int type;
	};

private:
	MessageHeader header;
	std::string msg;
	std::string body;

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & header.type;
		ar & body;
	}

public:
	Message() {}
	Message(MessageType type, std::string data, bool compressed = COMPRESSION);
	Message(std::string raw, bool compressed = COMPRESSION);

	void operator=(Message &rhs);

	std::string Get(){return msg;}
	MessageHeader Header(){return header;}
	std::string Body(){return body;}
	size_t Size(){return msg.size();}
	size_t HeaderSize(){return sizeof(MessageHeader);}
	size_t BodySize(){return body.size();}
};
