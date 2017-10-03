#include "Message.hpp"

Message::Message(MessageType type, std::string body, bool compressed)
{
	this->header = MessageHeader();
	this->header.type = type;
	this->body = body;

	// compress if enabled
	if(compressed)
		this->body = Compress(this->body);

	std::ostringstream oss;
	boost::archive::text_oarchive archive(oss);
	archive << *this;
	msg = oss.str();
}

Message::Message(std::string raw, bool compressed)
{
	this->msg = raw;
	std::istringstream iss(raw);
	boost::archive::text_iarchive archive(iss);
	archive >> *this;

	// decompress if enabled
	if(compressed)
		this->body = Decompress(this->body);
}

void Message::operator=(Message &rhs)
{
	memcpy(&header, &rhs.header, sizeof(MessageHeader));
	msg = rhs.msg;
	body = rhs.body;
}
